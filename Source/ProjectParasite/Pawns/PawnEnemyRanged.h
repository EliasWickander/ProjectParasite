// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemy.h"
#include "PawnEnemyRanged.generated.h"

class AProjectile;

/**
 * 
 */
UCLASS()
class PROJECTPARASITE_API APawnEnemyRanged : public APawnEnemy
{
	GENERATED_BODY()

public:
	APawnEnemyRanged();

	virtual void Attack() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* weaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USceneComponent* weaponSocket = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Type")
	TSubclassOf<AProjectile> projectile = nullptr;
};
