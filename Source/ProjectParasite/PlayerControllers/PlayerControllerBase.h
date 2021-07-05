// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UGameInstanceCustom;
class APawnBase;
class APawnParasite;
class AGameStateCustom;

UCLASS()
class PROJECTPARASITE_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	APawnParasite* GetPlayer() { return playerRef; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnFloorEnter();
	
	UFUNCTION()
	void OnFloorExit();
	
	void SetupInputBindings();
	void SetupGeneralActions();
	void SetupParasiteActions();
	void SetupEnemyActions();
	
	void MoveHorizontalInternal(float axis);
	void MoveVerticalInternal(float axis);
	void PossessInternal();
	void AttackInternal();
	void DashInternal();

	UGameInstanceCustom* gameInstanceRef = nullptr;
	APawnParasite* playerRef = nullptr;
	APawnBase* controlledPawn = nullptr;
	AGameStateCustom* gameStateRef = nullptr;
};
