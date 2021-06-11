// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemy.h"
#include "PawnEnemyRanged.generated.h"

class AWeapon;
class AProjectile;
class UWeaponComponent;

UCLASS()
class PROJECTPARASITE_API APawnEnemyRanged : public APawnEnemy
{
	GENERATED_BODY()

public:
	APawnEnemyRanged();

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void Attack() override;

protected:

	virtual void BeginPlay() override;

	AWeapon* weapon = nullptr;
};
