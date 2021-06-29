// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemy.h"
#include "RangedPawnEnemy.generated.h"

class ARangedWeapon;
class AProjectile;
class UWeaponComponent;

UCLASS()
class PROJECTPARASITE_API ARangedPawnEnemy : public APawnEnemy
{
	GENERATED_BODY()

public:
	ARangedPawnEnemy();

	virtual void UpdatePawnBehavior(float deltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack() override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<ARangedWeapon> weaponType;
};
