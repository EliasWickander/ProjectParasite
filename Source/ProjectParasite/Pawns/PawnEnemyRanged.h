// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemy.h"
#include "PawnEnemyRanged.generated.h"

class AProjectile;

UCLASS()
class PROJECTPARASITE_API APawnEnemyRanged : public APawnEnemy
{
	GENERATED_BODY()

public:
	APawnEnemyRanged();

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void Attack() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* weaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* weaponSocket = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AProjectile> projectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float fireRate = 2;

	float attackTimer = 0;
};
