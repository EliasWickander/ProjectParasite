// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateCustom.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorStartEvent);

class AEliminationGamemode;
class APawnParasite;

UCLASS()
class PROJECTPARASITE_API AGameStateCustom : public AGameStateBase
{
	GENERATED_BODY()

public:
	void OpenNextLevel();

	bool IsCurrentLevelLast();
	bool IsCurrentFloorLast();

	FString GetSubLevelName(int level, int floor);

	FOnFloorStartEvent OnFloorStart;
	
protected:
	virtual void BeginPlay() override;
	
private:

	void OnLevelLoaded();
	TMap<FString, int> levelMap;

	APawnParasite* playerRef = nullptr;
	AEliminationGamemode* gamemodeRef = nullptr;
	FString levelsDirectoryPath;
	int currentLevel = 1;
	int currentFloor = 1;
};
