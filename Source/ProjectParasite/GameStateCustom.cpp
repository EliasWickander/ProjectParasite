// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateCustom.h"

#include "Engine/LevelStreaming.h"
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

	PrepareLevelMap();
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

void AGameStateCustom::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Wait until next level has finished loading
	if(loadingNextFloor)
	{
		ULevelStreaming* nextLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor + 1));

		if(nextLevel->IsLevelLoaded())
		{
			//Place player on the player start on the new level
			PlacePlayerOnPlayerStart();

			currentFloor += 1;
			loadingNextFloor = false;
			
			OnFloorEnter();
			OnFloorEnterEvent.Broadcast();
		}
	}
}

void AGameStateCustom::OpenLevel(int level)
{
	if(DoesLevelExist(level, 1))
	{
		FString levelName = FString::Printf(TEXT("Level_%i"), level);
		
		UGameplayStatics::OpenLevel(GetWorld(), *levelName);
	}
}

void AGameStateCustom::PlacePlayerOnPlayerStart()
{
	AActor* playerStart = gamemodeRef->FindPlayerStart(playerRef->GetController());

	if(playerRef->GetPossessedEnemy())
	{
		playerRef->GetPossessedEnemy()->SetActorLocation(playerStart->GetActorLocation());
	}
	else
	{
		playerRef->SetActorLocation(playerStart->GetActorLocation());
	}
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

void AGameStateCustom::LoadNextFloor()
{
	FString currentScene = UGameplayStatics::GetCurrentLevelName(GetWorld());
	
	FString levelPrefix;
	FString currentLevelText;

	//Get current level as text
	currentScene.Split(TEXT("_"),&levelPrefix, &currentLevelText);

	//Get current level as integer
	currentLevel = UKismetStringLibrary::Conv_StringToInt(currentLevelText);

	if(!IsCurrentFloorLast())
	{
		FLatentActionInfo info;
		info.UUID = 1;

		//If player is possessing enemy, move it to the new level
		if(playerRef->GetPossessedEnemy())
		{
			ULevelStreaming* oldLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor));
			ULevelStreaming* nextLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor + 1));
			
			MoveActorToLevel(playerRef->GetPossessedEnemy(), oldLevel, nextLevel);
		}

		//Unload current sublevel
		UGameplayStatics::UnloadStreamLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor), info, true);

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

FString AGameStateCustom::GetSubLevelName(int level, int floor)
{
	return FString::Printf(TEXT("Level_%i_%i"), level, floor);
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
