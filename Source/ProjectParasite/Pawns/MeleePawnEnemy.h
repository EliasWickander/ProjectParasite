// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemy.h"
#include "MeleePawnEnemy.generated.h"

class AMeleeWeapon;
class UMeleeEnemyDebugComponent;

UCLASS()
class PROJECTPARASITE_API AMeleePawnEnemy : public APawnEnemy
{
	GENERATED_BODY()

public:
	AMeleePawnEnemy();
	virtual void BeginPlay() override;
	virtual void Attack() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMeleeEnemyDebugComponent* enemyDebugger = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<AMeleeWeapon> weaponType;
	
};
