// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateCustom.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorEnterEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorExitEvent);

class AEliminationGamemode;
class APawnParasite;

UCLASS()
class PROJECTPARASITE_API AGameStateCustom : public AGameStateBase
{
	GENERATED_BODY()

public:
	AGameStateCustom();
	virtual void Tick(float DeltaSeconds) override;
	void OpenNextLevel();

	bool IsCurrentLevelLast();
	bool IsCurrentFloorLast();

	FString GetSubLevelName(int level, int floor);

	FOnFloorEnterEvent OnFloorEnter;
	FOnFloorExitEvent OnFloorExit;
	
protected:
	virtual void BeginPlay() override;
	
private:
	TMap<FString, int> levelMap;

	APawnParasite* playerRef = nullptr;
	AEliminationGamemode* gamemodeRef = nullptr;
	FString levelsDirectoryPath;
	
	int currentLevel = 1;
	int currentFloor = 1;

	bool loadingNextLevel = false;
};
