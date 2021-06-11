// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class APawnBase;
class APawnParasite;

UCLASS()
class PROJECTPARASITE_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	void SetupInputBindings();
	void SetupGeneralActions();
	void SetupParasiteActions();
	void SetupEnemyActions();
	
	void MoveHorizontalInternal(float axis);
	void MoveVerticalInternal(float axis);
	void UnpossessInternal();
	void AttackInternal();
	void DashInternal();

	APawnParasite* player = nullptr;
	APawnBase* controlledPawn = nullptr;
};
