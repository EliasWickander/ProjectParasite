// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponBase.h"

#include "CoreMinimal.h"
#include "RangedWeapon.generated.h"

class AProjectile;

UCLASS()
class PROJECTPARASITE_API ARangedWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Trigger() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* weaponSocket = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectile> projectile = nullptr;
};
