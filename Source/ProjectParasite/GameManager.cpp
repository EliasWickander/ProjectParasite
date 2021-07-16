// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "Engine/LevelStreaming.h"
#include "GameFramework/PlayerStart.h"
#include "ProjectParasite/GameModes/EliminationGamemode.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Pawns/PawnEnemy.h"
#include "Utilities/DevUtils.h"

//TODO: Handle restarting floors by teleporting everything to its original position instead of trying to load sublevel?
UGameManager::UGameManager()
{
	PrepareLevelMap();
}

void UGameManager::BeginPlay()
{
	levelsDirectoryPath = FString::Printf(TEXT("%s/Levels"), *FPaths::ProjectContentDir());

	gamemodeRef = Cast<AEliminationGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	currentWorldName = UGameplayStatics::GetCurrentLevelName(GetWorld());
}

void UGameManager::Tick(float DeltaSeconds)
{
	if(isLoadingLevel)
	{
		OnLoadingLevel();
	}

	if(isLoadingFloor)
	{
		OnLoadingFloor();
	}
}

void UGameManager::OpenLevel(int level, int floor)
{
	//Opens level if it exists
	if(DoesLevelExist(level, floor))
	{
		FString levelName = FString::Printf(TEXT("Level_%i"), level);
		
		UGameplayStatics::OpenLevel(GetWorld(), *levelName);

		nextLevel = level;

		nextFloor = floor;
		
		isLoadingLevel = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to open level %i floor %i that doesn't exist"), level, floor);
	}
}

void UGameManager::OpenLevelNew(int level, int floor)
{
	FString levelName = FString::Printf(TEXT("Level_%i_%i"), level, floor);

	UWorld* currentWorld = playerRef->GetWorld();

	UGameplayStatics::OpenLevel(GetWorld(), *levelName);

	//find a way to get new world
	UWorld* newWorld = playerRef->GetWorld();

	UE_LOG(LogTemp, Warning, TEXT("%s, %s"), *currentWorld->GetName(), *newWorld->GetName());
	
	//If player is possessing enemy, move it to the new level
	if(playerRef->GetPossessedEnemy())
	{
		
		//Moves possessed enemy to new floor (Remember that all floors need to have the "Initially loaded" checkbox ticked for this to work)
		//MoveActorToLevel(playerRef->GetPossessedEnemy(), currentLevelStreaming, nextLevelStreaming);
	}
}

void UGameManager::LoadFloor(int floor)
{
	ULevelStreaming* currentLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor));
	ULevelStreaming* nextLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, floor));
	
	if(DoesLevelExist(currentLevel, floor))
	{
		if(nextFloor != currentFloor)
		{
			//If player is possessing enemy, move it to the new level
			if(playerRef->GetPossessedEnemy())
			{
				//Moves possessed enemy to new floor (Remember that all floors need to have the "Initially loaded" checkbox ticked for this to work)
				MoveActorToLevel(playerRef->GetPossessedEnemy(), currentLevelStreaming, nextLevelStreaming);
			}
	
			FLatentActionInfo info;
			info.UUID = 1;
	
			if(currentLevelStreaming)
			{
				UGameplayStatics::UnloadStreamLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor), info, true);
	
				UE_LOG(LogTemp, Warning, TEXT("Unload %i, %i"), currentLevel, currentFloor);
			}
	
			//Broadcast the floor exit
			OnFloorExit();
			OnFloorExitEvent.Broadcast(floor);
	
			info.UUID = 2;
	
			//Load next sublevel
			UGameplayStatics::LoadStreamLevel(GetWorld(), *GetSubLevelName(currentLevel, floor), true, true, info);	
		}
	
		nextFloor = floor;
	
		isLoadingFloor = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to load floor %i but it doesn't exist"), floor)
	 }
}

void UGameManager::RestartFloor()
{
	OpenLevel(currentLevel, currentFloor);
}

void UGameManager::OnLoadingFloor()
{
	ULevelStreaming* nextLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, nextFloor));

	if(nextLevelStreaming)
	{
		if(nextLevelStreaming->IsLevelLoaded() && nextLevelStreaming->IsLevelVisible())
		{
			currentFloor = nextFloor;

			UE_LOG(LogTemp, Warning, TEXT("Loaded floor"));
			PlacePlayerOnPlayerStart();
				
			OnFloorEnter();
			OnFloorEnterEvent.Broadcast(currentFloor);

			isLoadingFloor = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to load next floor %i of level %i but doesn't exist"), nextFloor, currentLevel);
	}
}

void UGameManager::OnLoadingLevel()
{
	ULevelStreaming* currentLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(nextLevel, 1));

	if(currentLevelStreaming)
	{
		if(currentLevelStreaming->IsLevelLoaded() && currentLevelStreaming->IsLevelVisible())
		{
			currentLevel = nextLevel;

			currentFloor = 1;
			UE_LOG(LogTemp, Warning, TEXT("Loaded level"));

			LoadFloor(nextFloor);
			
			isLoadingLevel = false;
		}	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to load next level %i but doesn't exist"), nextLevel);
	}
}

void UGameManager::PlacePlayerOnPlayerStart()
{
	ULevelStreaming* currentLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor));

	AActor* playerStart = nullptr;

	if(currentLevelStreaming)
	{
		//Iterate over all actors in current floor to find the player start
		if(currentLevelStreaming->IsLevelLoaded() && currentLevelStreaming->IsLevelVisible()) 
		{
			for(AActor* actor : currentLevelStreaming->GetLoadedLevel()->Actors)
			{
				if(Cast<APlayerStart>(actor))
				{
					playerStart = actor;
				}
			}	
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Trying to place player on player start but sublevel is not yet loaded"))
			return;
		}	
	}

	//Place player and possible possessed enemy at player start if found
	if(playerStart)
	{
		if(playerRef->GetPossessedEnemy())
		{
			playerRef->GetPossessedEnemy()->SetActorLocation(playerStart->GetActorLocation());
		}
		else
		{
			playerRef->SetActorLocation(playerStart->GetActorLocation());
		}	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Level %i has no player start"), currentLevel);
	}
}

void UGameManager::LoadNextFloor()
{
	LoadFloor(currentFloor + 1);
}

void UGameManager::LoadPrevFloor()
{
	LoadFloor(currentFloor - 1);
}

bool UGameManager::DoesLevelExist(int level, int floor)
{
	FString levelToCheck = GetSubLevelName(level, floor);

	return FPaths::FileExists(FString::Printf(TEXT("%s/%s.umap"), *levelsDirectoryPath, *levelToCheck));
}

int UGameManager::GetLevelAmount()
{
	return levelMap.Num();
}

int UGameManager::GetFloorAmount(int level)
{
	if(levelMap.Contains(level))
	{
		return levelMap[level].Num();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to get floor amount of level %i that doesn't exist"), level);
		return -1;
	}
}

bool UGameManager::IsCurrentFloorLast()
{
	return !DoesLevelExist(currentLevel, currentFloor + 1);
}

bool UGameManager::IsCurrentLevelLast()
{
	return !DoesLevelExist(currentLevel + 1, 1);
}

void UGameManager::PrepareLevelMap()
{
	//Add all available levels and floors to a map of arrays
	//The key represents the level, and the array its floors
	
	int levelAmount = 0;

	while(FPaths::FileExists(FString::Printf(TEXT("%s/Level_%i.umap"), *levelsDirectoryPath, levelAmount + 1)))
	{
		int floorAmount = 0;
		TArray<int> floorArray;

		while(FPaths::FileExists(FString::Printf(TEXT("%s/Level_%i_%i.umap"), *levelsDirectoryPath, levelAmount + 1, floorAmount + 1)))
		{
			floorArray.Add(floorAmount);

			floorAmount++;
		}
		
		levelAmount++;
	}
}

FString UGameManager::GetSubLevelName(int level, int floor)
{
	return FString::Printf(TEXT("Level_%i_%i"), level, floor);
}
