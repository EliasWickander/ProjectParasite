// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateCustom.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorEnterEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorExitEvent);

class UGameInstanceCustom;
class AEliminationGamemode;
class APawnParasite;

UCLASS()
class PROJECTPARASITE_API AGameStateCustom : public AGameStateBase
{
	GENERATED_BODY()

public:
	AGameStateCustom();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void OpenLevel(int level);
	
	void LoadNextFloor();

	FString GetSubLevelName(int level, int floor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnFloorEnter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFloorExit();

	UFUNCTION(BlueprintCallable)
	bool DoesLevelExist(int level, int floor);

	bool IsCurrentFloorLast();
	bool IsCurrentLevelLast();
	
	FOnFloorEnterEvent OnFloorEnterEvent;
	FOnFloorExitEvent OnFloorExitEvent;
	
protected:
	virtual void BeginPlay() override;
	
private:
	void PrepareLevelMap();
	void PlacePlayerOnPlayerStart();

	UFUNCTION(BlueprintCallable)
	int GetLevelAmount();

	UFUNCTION(BlueprintCallable)
	int GetFloorAmount(int level);

	TMap<int, TArray<int>> levelMap;

	int currentLevel = 1;
	int currentFloor = 1;

	FString levelsDirectoryPath;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool loadingNextFloor = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APawnParasite* playerRef = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AEliminationGamemode* gamemodeRef = nullptr;
};
