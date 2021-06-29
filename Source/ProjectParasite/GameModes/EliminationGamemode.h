// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EliminationGamemode.generated.h"

class APawnEnemy;
class APawnBase;
class APawnParasite;

UCLASS()
class PROJECTPARASITE_API AEliminationGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnGameWon();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnGameLost();

	bool HasEliminatedAllEnemies();

private:
	UFUNCTION()
	void OnEnemyDeath(APawnBase* deadEnemy, const UDamageType* damageType);

	UFUNCTION()
	void OnPlayerDeath(APawnBase* deadPlayer, const UDamageType* damageType);

	APawnParasite* playerRef = nullptr;
	TArray<AActor*> enemiesAlive = {};

	int score = 0;
	
};
