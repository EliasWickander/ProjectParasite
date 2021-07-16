// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameManager.generated.h"

class APawnParasite;
class AEliminationGamemode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloorEnterEvent, int, floor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloorExitEvent, int, floor);

UCLASS()
class PROJECTPARASITE_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	UGameManager();
	void BeginPlay();
	void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void OpenLevel(int level, int floor);

	UFUNCTION(BlueprintCallable)
	void OpenLevelNew(int level, int floor);
	
	UFUNCTION(BlueprintCallable)
	void LoadFloor(int floor);

	UFUNCTION(BlueprintCallable)
	void RestartFloor();

	void LoadNextFloor();
	void LoadPrevFloor();

	void OnLoadingFloor();
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
	
	void PlacePlayerOnPlayerStart();

	void OnLoadingLevel();
	
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

	bool isLoadingLevel = false;
	bool isLoadingFloor = false;
	
	FString currentWorldName = "";
	FString levelsDirectoryPath;
};
