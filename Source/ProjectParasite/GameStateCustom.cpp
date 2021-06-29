// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateCustom.h"

#include "Kismet/GameplayStatics.h"

void AGameStateCustom::BeginPlay()
{
	Super::BeginPlay();

	levelsDirectoryPath = FString::Printf(TEXT("%s/Levels"), *FPaths::ProjectContentDir());

	for(int i = 0; i < 100; i++)
	{
		FString key = FString::FromInt(i);
		
		int val = i;
		
		levelMap.Add({key}, {val});
	}
}

void AGameStateCustom::OpenLevel(FString levelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

void AGameStateCustom::OpenNextLevel()
{
	FString currentScene = UGameplayStatics::GetCurrentLevelName(GetWorld());
	
	FString levelPrefix;
	FString levelInfo;
	
	currentScene.Split(TEXT("_"),&levelPrefix, &levelInfo);
	
	FString currentLevelText;
	FString currentFloorText;
	
	levelInfo.Split(TEXT("_"), &currentLevelText, &currentFloorText);

	if(levelMap.Contains(currentLevelText) && levelMap.Contains(currentFloorText))
	{
		currentLevel = levelMap[currentLevelText];
		currentFloor = levelMap[currentFloorText];

		if(!IsCurrentFloorLast())
		{
			FString levelName = FString::Printf(TEXT("Level_%i_%i"), currentLevel, currentFloor + 1);
			
			OpenLevel(*levelName);
		}
		else
		{
			if(!IsCurrentLevelLast())
			{
				FString levelName = FString::Printf(TEXT("Level_%i_%i"), currentLevel + 1, 1);
			
				OpenLevel(*levelName);
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
	FString nextScene = FString::Printf(TEXT("Level_%i_%i"), currentLevel + 1, 1);
		
	return !FPaths::FileExists(FString::Printf(TEXT("%s/%s.umap"), *levelsDirectoryPath, *nextScene));
}

bool AGameStateCustom::IsCurrentFloorLast()
{
	FString nextScene = FString::Printf(TEXT("Level_%i_%i"), currentLevel, currentFloor + 1);
		
	return !FPaths::FileExists(FString::Printf(TEXT("%s/%s.umap"), *levelsDirectoryPath, *nextScene));
}