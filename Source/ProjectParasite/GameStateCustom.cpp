// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateCustom.h"

#include "ProjectParasite/GameModes/EliminationGamemode.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "Kismet/GameplayStatics.h"

void AGameStateCustom::BeginPlay()
{
	Super::BeginPlay();

	gamemodeRef = Cast<AEliminationGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	levelsDirectoryPath = FString::Printf(TEXT("%s/Levels"), *FPaths::ProjectContentDir());

	for(int i = 0; i < 100; i++)
	{
		FString key = FString::FromInt(i);
		
		int val = i;
		
		levelMap.Add({key}, {val});
	}	

	//UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Level_1_1"), true, false, info);
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
			
			//Unload current sublevel
			UGameplayStatics::UnloadStreamLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor), info, false);
			
			info.UUID = 2;
			//Load next sublevel
			UGameplayStatics::LoadStreamLevel(GetWorld(), *GetSubLevelName(currentLevel, currentFloor + 1), true, false, info);

			OnFloorStart.Broadcast();
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
