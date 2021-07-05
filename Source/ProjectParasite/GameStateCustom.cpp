// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateCustom.h"

#include "Engine/LevelStreaming.h"
#include "ProjectParasite/GameModes/EliminationGamemode.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "Kismet/GameplayStatics.h"
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

	for(int i = 0; i < 100; i++)
		levelMap.Add(FString::FromInt(i), i);
}

void AGameStateCustom::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Wait until next level has finished loading
	if(loadingNextLevel)
	{
		ULevelStreaming* nextLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor + 1));

		if(nextLevel->IsLevelLoaded())
		{
			//Place player on the player start on the new level
			PlacePlayerOnPlayerStart();

			currentFloor += 1;
			loadingNextLevel = false;
			
			OnFloorEnter();
			OnFloorEnterEvent.Broadcast();
		}
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

void AGameStateCustom::OpenNextLevel()
{
	FString currentScene = UGameplayStatics::GetCurrentLevelName(GetWorld());
	
	FString levelPrefix;
	FString currentLevelText;

	//Get current level as text
	currentScene.Split(TEXT("_"),&levelPrefix, &currentLevelText);

	if(levelMap.Contains(currentLevelText))
	{
		//Get current level as integer
		currentLevel = levelMap[currentLevelText];

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

			loadingNextLevel = true;
		}
		else
		{
			//Last floor
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Levelname does not match naming rules. The correct naming is {Level_{LevelNumber}_{FloorNumber}"));
	}
	
}

bool AGameStateCustom::IsCurrentLevelLast()
{
	FString nextLevel = GetSubLevelName(currentLevel + 1, 1);
		
	return !FPaths::FileExists(FString::Printf(TEXT("%s/%s.umap"), *levelsDirectoryPath, *nextLevel));
}

bool AGameStateCustom::IsCurrentFloorLast()
{
	FString nextLevel = GetSubLevelName(currentLevel, currentFloor + 1);
		
	return !FPaths::FileExists(FString::Printf(TEXT("%s/%s.umap"), *levelsDirectoryPath, *nextLevel));
}

FString AGameStateCustom::GetSubLevelName(int level, int floor)
{
	return FString::Printf(TEXT("Level_%i_%i"), level, floor);
}
