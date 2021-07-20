// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameManager.generated.h"

class APawnParasite;
class AEliminationGamemode;
class APawnEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloorEnterEvent, int, floor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloorExitEvent, int, floor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseGameEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnpauseGameEvent);

enum GameState
{
	Playing,
	Paused,
	FloorEnter,
	FloorExit,
};
UCLASS()
class PROJECTPARASITE_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	void BeginPlay();
	void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void OpenLevel(int level, int floor);
	
	UFUNCTION(BlueprintCallable)
	bool DoesLevelExist(int level, int floor);

	bool IsCurrentFloorLast();

	void LoadNextFloor();

	bool IsOnFloorLevel();
	
	UFUNCTION(BlueprintCallable)
	void RestartLevel();

	int GetCurrentFloor();
	int GetCurrentLevel();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFloorEnter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFloorExit();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPauseGame();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnpauseGame();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTick(float deltaTime);

	bool GetPaused() {return isPaused; }
	void SetPaused(bool paused);
	FOnFloorEnterEvent OnFloorEnterEvent;
	FOnFloorExitEvent OnFloorExitEvent;
	FOnPauseGameEvent OnPauseGameEvent;
	FOnUnpauseGameEvent OnUnpauseGameEvent;

private:
	bool loadingFirstFloor = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APawnParasite* playerRef = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AEliminationGamemode* gamemodeRef = nullptr;

	bool isLoadingLevel = false;
	bool isLoadingFloor = false;

	FString nextLevel = "";
	
	FString currentWorldName = "";

	UClass* possessedEnemyToTransition = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool transitionOutOfLevel = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool transitionIntoLevel = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float levelTransitionTimer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float levelTransitionTime = 0.5f;
	
	bool beginPlayTriggered = false;

	bool isPaused = false;
};
