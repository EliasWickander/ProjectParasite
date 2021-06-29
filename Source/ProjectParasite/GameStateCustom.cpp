// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateCustom.h"

#include "Kismet/GameplayStatics.h"

void AGameStateCustom::BeginPlay()
{
	Super::BeginPlay();

	for(int i = 0; i < 100; i++)
	{
		FString key = FString::FromInt(i);
		
		int val = i;
		
		levelMap.Add({key}, {val});
	}
	OpenNextLevel();
}

void AGameStateCustom::OpenNextLevel()
{
	FString currentScene = UGameplayStatics::GetCurrentLevelName(GetWorld());
	
	FString levelPrefix;
	FString levelInfo;
	
	currentScene.Split(TEXT("_"),&levelPrefix, &levelInfo);
	
	FString currentLevel;
	FString currentFloor;
	
	levelInfo.Split(TEXT("_"), &currentLevel, &currentFloor);

	FString floorPrefix;
	FString floorNumber;

	currentFloor.Split(TEXT("F"))

	if(levelMap.Contains(currentLevel))
	{
		
		UE_LOG(LogTemp, Warning, TEXT("%i"), levelMap[currentLevel]);	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Levelname does not match naming rules. The correct naming is {Level_{LevelNumber}_{FloorNumber}"));
	}
	
}