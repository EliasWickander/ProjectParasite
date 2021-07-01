// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EliminationGamemode.generated.h"

class APawnEnemy;
class APawnBase;
class APawnParasite;
class AWeaponBase;
class AGoalTrigger;
class AGameStateCustom;

UCLASS()
class PROJECTPARASITE_API AEliminationGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEliminationGamemode();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnFloorStart();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnGameWon();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnGameLost();

	bool HasEliminatedAllEnemies();

private:
	UFUNCTION()
	void OnEnemyDeath(APawnBase* deadEnemy, AActor* causerActor);

	UFUNCTION()
	void OnPlayerDeath(APawnBase* deadPlayer, AActor* causerActor);

	UFUNCTION()
	void OnGoalTriggered();
	
	void AddScore(float scoreToAdd, bool allowCombo = true);

	AGameStateCustom* gameStateRef = nullptr;
	APawnParasite* playerRef = nullptr;
	
	TArray<AActor*> enemiesAlive = {};

	AGoalTrigger* goalTrigger = nullptr;
	int currentScore = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	float comboWindow = 4;
	float comboTimer = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	float comboScore = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	float finishOnTimeScore = 2000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float levelTimeLimit = 2;
	float levelTimer = 0;
	
};
