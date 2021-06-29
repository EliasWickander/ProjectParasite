// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateCustom.generated.h"

UCLASS()
class PROJECTPARASITE_API AGameStateCustom : public AGameStateBase
{
	GENERATED_BODY()

public:
	void OpenLevel(FString levelName);
	void OpenNextLevel();

	bool IsCurrentLevelLast();
	bool IsCurrentFloorLast();
	
protected:
	virtual void BeginPlay() override;
	
private:
	TMap<FString, int> levelMap;

	FString levelsDirectoryPath;
	int currentLevel;
	int currentFloor;
};
