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

	if(currentLevel == -1)
		FindCurrentLevel();
}

void UGameManager::Tick(float DeltaSeconds)
{
	if(nextFloor != currentFloor)
	{
		if(nextFloor != -1)
			PrepareNextFloor();
	}

	if(nextLevel != currentLevel)
	{
		if(nextLevel != -1)
			PrepareNextLevel();
	}
}

void UGameManager::OpenLevel(int level)
{
	//Opens level if it exists
	if(DoesLevelExist(level, 1))
	{
		FString levelName = FString::Printf(TEXT("Level_%i"), level);
		
		UGameplayStatics::OpenLevel(GetWorld(), *levelName);

		nextLevel = level;

		//PlacePlayerOnPlayerStart();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to open level %i that doesn't exist"), level);
	}
}

void UGameManager::LoadFloor(int floor)
{
	if(DoesLevelExist(currentLevel, floor))
	{
		//If player is possessing enemy, move it to the new level
		if(playerRef->GetPossessedEnemy())
		{
			ULevelStreaming* currentLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor));
			ULevelStreaming* nextLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, floor));

			//Moves possessed enemy to new floor (Remember that all floors need to have the "Initially loaded" checkbox ticked for this to work)
			MoveActorToLevel(playerRef->GetPossessedEnemy(), currentLevelStreaming, nextLevelStreaming);
		}

		FLatentActionInfo info;
		info.UUID = 1;

		//Unload current sublevel
		UGameplayStatics::UnloadStreamLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor), info, true);

		//Broadcast the floor exit
		OnFloorExit();
		OnFloorExitEvent.Broadcast();
	
		info.UUID = 2;
		
		//Load next sublevel
		UGameplayStatics::LoadStreamLevel(GetWorld(), *GetSubLevelName(currentLevel, floor), true, true, info);

		nextFloor = floor;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to load floor %i but it doesn't exist"), floor)
	}
}

void UGameManager::PrepareNextLevel()
{
	ULevelStreaming* currentLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(nextLevel, 1));

	if(currentLevelStreaming->IsLevelLoaded() && currentLevelStreaming->IsLevelVisible())
	{
		currentLevel = nextLevel;
		currentFloor = 1;
		
		PlacePlayerOnPlayerStart();
	}
}

void UGameManager::PrepareNextFloor()
{
	ULevelStreaming* nextLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, nextFloor));

	if(nextLevelStreaming)
	{
		if(nextLevelStreaming->IsLevelLoaded() && nextLevelStreaming->IsLevelVisible())
		{
			currentFloor = nextFloor;

			//Place player on the player start on the new level
			PlacePlayerOnPlayerStart();
		
			OnFloorEnter();
			OnFloorEnterEvent.Broadcast();
		}	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There is no level %i with floor %i"), currentLevel, nextFloor)
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
				UE_LOG(LogTemp, Warning, TEXT("%s - %i"), *actor->GetName(), Cast<APlayerStart>(actor));
				
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

int UGameManager::FindCurrentLevel()
{
	FString levelPrefix;
	FString currentLevelText;

	//Get current level as text
	currentWorldName.Split(TEXT("_"),&levelPrefix, &currentLevelText);

	//Get current level as integer
	currentLevel = UKismetStringLibrary::Conv_StringToInt(currentLevelText);

	return currentLevel;
}

FString UGameManager::GetSubLevelName(int level, int floor)
{
	return FString::Printf(TEXT("Level_%i_%i"), level, floor);
}
