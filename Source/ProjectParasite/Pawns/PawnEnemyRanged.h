// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemy.h"
#include "PawnEnemyRanged.generated.h"

class AWeaponBase;
class AProjectile;
class UWeaponComponent;

UCLASS()
class PROJECTPARASITE_API APawnEnemyRanged : public APawnEnemy
{
	GENERATED_BODY()

public:
	APawnEnemyRanged();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack() override;

	virtual void UpdatePawnBehavior(float deltaSeconds) override;

protected:

	virtual void BeginPlay() override;
};
