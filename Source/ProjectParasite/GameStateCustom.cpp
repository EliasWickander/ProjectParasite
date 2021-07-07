// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateCustom.h"

#include "Engine/LevelStreaming.h"
#include "GameFramework/PlayerStart.h"
#include "ProjectParasite/GameModes/EliminationGamemode.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Pawns/PawnEnemy.h"
#include "Utilities/DevUtils.h"

AGameStateCustom::AGameStateCustom()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameStateCustom::BeginPlay()
{
	Super::BeginPlay();

	gamemodeRef = Cast<AEliminationGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	levelsDirectoryPath = FString::Printf(TEXT("%s/Levels"), *FPaths::ProjectContentDir());

	currentWorldName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	
	PrepareLevelMap();

	FindCurrentLevel();
	
	loadingFirstFloor = true;
}

void AGameStateCustom::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(loadingFirstFloor)
	{
		PrepareFirstFloor();
	}
	else if(loadingNextFloor)
	{
		PrepareNextFloor();
	}
}

void AGameStateCustom::LoadNextFloor()
{
	if(!IsCurrentFloorLast())
	{
		//If player is possessing enemy, move it to the new level
		if(playerRef->GetPossessedEnemy())
		{
			ULevelStreaming* oldLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor));
			ULevelStreaming* nextLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor + 1));

			//Moves possessed enemy to new floor (Remember that all floors need to have the "Initially loaded" checkbox ticked for this to work)
			MoveActorToLevel(playerRef->GetPossessedEnemy(), oldLevel, nextLevel);
			
			UE_LOG(LogTemp, Warning, TEXT("%s"), *playerRef->GetPossessedEnemy()->GetOutermostObject()->GetName());
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
		UGameplayStatics::LoadStreamLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor + 1), true, true, info);

		loadingNextFloor = true;
	}
	else
	{
		//Last floor
	}
	
}

void AGameStateCustom::OpenLevel(int level)
{
	//Opens level if it exists
	if(DoesLevelExist(level, 1))
	{
		FString levelName = FString::Printf(TEXT("Level_%i"), level);
		
		UGameplayStatics::OpenLevel(GetWorld(), *levelName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to open level %i that doesn't exist"), level);
	}
}

void AGameStateCustom::PlacePlayerOnPlayerStart()
{
	ULevelStreaming* currentLevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor));

	AActor* playerStart = nullptr;

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
		UE_LOG(LogTemp, Error, TEXT("This level has no player start"));
	}
}

void AGameStateCustom::PrepareFirstFloor()
{
	ULevelStreaming* loadingLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, 1));

	//Place player on player start when loading first floor
	if(loadingLevel)
	{
		if(loadingLevel->IsLevelLoaded() && loadingLevel->IsLevelVisible())
		{
			loadingFirstFloor = false;

			PlacePlayerOnPlayerStart();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There is no level %i with floor 1"), currentLevel)
	}
}

void AGameStateCustom::PrepareNextFloor()
{
	ULevelStreaming* nextLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor + 1));

	if(nextLevel)
	{
		if(nextLevel->IsLevelLoaded() && nextLevel->IsLevelVisible())
		{
			loadingNextFloor = false;

			currentFloor += 1;

			//Place player on the player start on the new level
			PlacePlayerOnPlayerStart();
		
			OnFloorEnter();
			OnFloorEnterEvent.Broadcast();
		}	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There is no level %i with floor %i"), currentLevel, currentFloor + 1)
	}
}

void AGameStateCustom::PrepareLevelMap()
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

int AGameStateCustom::FindCurrentLevel()
{	
	FString levelPrefix;
	FString currentLevelText;

	//Get current level as text
	currentWorldName.Split(TEXT("_"),&levelPrefix, &currentLevelText);

	//Get current level as integer
	currentLevel = UKismetStringLibrary::Conv_StringToInt(currentLevelText);

	return currentLevel;
}

FString AGameStateCustom::GetSubLevelName(int level, int floor)
{
	return FString::Printf(TEXT("Level_%i_%i"), level, floor);
}

int AGameStateCustom::GetLevelAmount()
{
	return levelMap.Num();
}

int AGameStateCustom::GetFloorAmount(int level)
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

bool AGameStateCustom::DoesLevelExist(int level, int floor)
{
	FString levelToCheck = GetSubLevelName(level, floor);

	return FPaths::FileExists(FString::Printf(TEXT("%s/%s.umap"), *levelsDirectoryPath, *levelToCheck));
}

bool AGameStateCustom::IsCurrentLevelLast()
{
	return !DoesLevelExist(currentLevel + 1, 1);
}

bool AGameStateCustom::IsCurrentFloorLast()
{
	return !DoesLevelExist(currentLevel, currentFloor + 1);
}


