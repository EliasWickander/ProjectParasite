// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class APawnBase;
class APawnParasite;
class AGameStateCustom;
class UGameManager;

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
	void SetupInputBindings();
	void SetupGeneralActions();
	void SetupParasiteActions();
	void SetupEnemyActions();
	
	void MoveHorizontalInternal(float axis);
	void MoveVerticalInternal(float axis);
	void PossessInternal();
	void AttackInternal();
	void DashInternal();

	UFUNCTION()
	void OnGamePaused();
	
	UFUNCTION()
	void OnGameUnpaused();

	UPROPERTY(BlueprintReadOnly)
	APawnParasite* playerRef = nullptr;

	UPROPERTY(BlueprintReadOnly)
	APawnBase* controlledPawn = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AGameStateCustom* gameStateRef = nullptr;

	UGameManager* gameManagerRef = nullptr;
};
