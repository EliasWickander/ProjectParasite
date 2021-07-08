// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameManager.generated.h"

class APawnParasite;
class AEliminationGamemode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorEnterEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorExitEvent);

UCLASS()
class PROJECTPARASITE_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	UGameManager();
	void BeginPlay();
	void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void OpenLevel(int level);

	UFUNCTION(BlueprintCallable)
	void LoadFloor(int floor);

	void LoadNextFloor();
	void LoadPrevFloor();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFloorEnter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFloorExit();
	
	FOnFloorEnterEvent OnFloorEnterEvent;
	FOnFloorExitEvent OnFloorExitEvent;
	
	UFUNCTION(BlueprintCallable)
	bool DoesLevelExist(int level, int floor);

	UFUNCTION(BlueprintCallable)
	int GetLevelAmount();

	UFUNCTION(BlueprintCallable)
	int GetFloorAmount(int level);

	bool IsCurrentFloorLast();
	bool IsCurrentLevelLast();

private:
	void PrepareLevelMap();
	int FindCurrentLevel();
	
	void PlacePlayerOnPlayerStart();

	void PrepareNextLevel();
	void PrepareNextFloor();
	
	FString GetSubLevelName(int level, int floor);

	bool loadingFirstFloor = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APawnParasite* playerRef = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AEliminationGamemode* gamemodeRef = nullptr;
	TMap<int, TArray<int>> levelMap;
	int currentLevel = -1;
	int currentFloor = -1;
	int nextLevel = -1;
	int nextFloor = -1;
	
	FString currentWorldName = "";
	FString levelsDirectoryPath;
};
