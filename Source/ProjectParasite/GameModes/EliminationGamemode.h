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
class UGameManager;

UCLASS()
class PROJECTPARASITE_API AEliminationGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEliminationGamemode();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnFloorEnter(int floor);

	UFUNCTION()
	void OnFloorExit(int floor);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnFloorFinished();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnPlayerDied();

	float GetLevelTimeLimit() { return levelTimeLimit; }

	bool HasEliminatedAllEnemies();

	void HandleWinCondition();
	void RemoveFromEnemiesAlive(AActor* enemyToRemove) {enemiesAlive.Remove(enemyToRemove);}


private:
	UFUNCTION()
	void OnEnemyDeath(APawnBase* deadEnemy, AActor* causerActor);

	UFUNCTION()
	void OnPlayerDeath(APawnBase* deadPlayer, AActor* causerActor);

	UFUNCTION()
	void OnGoalTriggered();

	UFUNCTION(BlueprintCallable)
	AGoalTrigger* FindGoalTrigger();

	UGameManager* gameManagerRef = nullptr;
	APawnParasite* playerRef = nullptr;
	
	TArray<AActor*> enemiesAlive = {};

	AGoalTrigger* goalTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	float comboWindow = 4;
	float comboTimer = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	float comboScore = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	float finishOnTimeScore = 2000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float levelTimeLimit = 2;
	
};
