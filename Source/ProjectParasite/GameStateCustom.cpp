// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateCustom.h"

#include "Engine/LevelStreaming.h"
#include "ProjectParasite/GameModes/EliminationGamemode.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/GameInstanceCustom.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PawnEnemy.h"
#include "Utilities/DevUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/PostProcessComponent.h"

AGameStateCustom::AGameStateCustom()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AGameStateCustom::BeginPlay()
{
	Super::BeginPlay();

	gameInstanceRef = Cast<UGameInstanceCustom>(UGameplayStatics::GetGameInstance(GetWorld()));
	gamemodeRef = Cast<AEliminationGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	levelsDirectoryPath = FString::Printf(TEXT("%s/Levels"), *FPaths::ProjectContentDir());

	for(int i = 0; i < 100; i++)
	{
		FString key = FString::FromInt(i);
		
		int val = i;
		
		levelMap.Add({key}, {val});
	}
}

void AGameStateCustom::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
		
	if(loadingNextLevel)
	{
		ULevelStreaming* nextLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor + 1));

		if(nextLevel->IsLevelLoaded())
		{
			PlacePlayerOnPlayerStart();
			
			OnFloorEnter();
			OnFloorEnterEvent.Broadcast();
			
			currentFloor += 1;
			loadingNextLevel = false;
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
	
	currentScene.Split(TEXT("_"),&levelPrefix, &currentLevelText);
	
	if(levelMap.Contains(currentLevelText))
	{
		currentLevel = levelMap[currentLevelText];

		if(!IsCurrentFloorLast())
		{
			FLatentActionInfo info;
			info.UUID = 1;

			if(playerRef->GetPossessedEnemy())
			{
				ULevelStreaming* oldLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor));
				ULevelStreaming* nextLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor + 1));
				
				MoveActorToLevel(playerRef->GetPossessedEnemy(), oldLevel, nextLevel);
			}
		
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
			if(!IsCurrentLevelLast())
			{
				UGameplayStatics::OpenLevel(GetWorld(), *GetSubLevelName(currentLevel + 1, 1));
			}
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
