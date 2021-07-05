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
	void OpenNextLevel();

	bool IsCurrentLevelLast();
	bool IsCurrentFloorLast();

	FString GetSubLevelName(int level, int floor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnFloorEnter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFloorExit();
	
	FOnFloorEnterEvent OnFloorEnterEvent;
	FOnFloorExitEvent OnFloorExitEvent;
	
protected:
	virtual void BeginPlay() override;
	
private:

	void PlacePlayerOnPlayerStart();
	TMap<FString, int> levelMap;

	UGameInstanceCustom* gameInstanceRef = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APawnParasite* playerRef = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AEliminationGamemode* gamemodeRef = nullptr;
	
	FString levelsDirectoryPath;
	
	int currentLevel = 1;
	int currentFloor = 1;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool loadingNextLevel = false;
};
