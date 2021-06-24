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
	
	ARangedWeapon();

	virtual void Tick(float DeltaTime) override;

	virtual void Trigger() override;

	virtual void Reload();

	int GetCurrentAmmo() { return currentAmmo; }
	int GetMaxAmmo() { return maxAmmo; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* weaponSocket = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<AProjectile> projectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int maxAmmo = 6;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
	int currentAmmo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
	float timeToReload = 1;
	float reloadTimer = 0;
};
