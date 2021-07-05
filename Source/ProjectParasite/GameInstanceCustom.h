// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceCustom.generated.h"

class APawnParasite;
class APawnEnemy;
class AGameStateCustom;

struct EnemySpawnData
{
	TSubclassOf<APawnEnemy> enemyType;
	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;
};

UCLASS()
class PROJECTPARASITE_API UGameInstanceCustom : public UGameInstance
{
	GENERATED_BODY()

public:
	void OnFloorEnter();
	
	void TransferEnemyToNextLevel(APawnEnemy* enemy);

private:
	AGameStateCustom* gameStateRef = nullptr;

	TArray<EnemySpawnData> enemiesToTransfer;
};
