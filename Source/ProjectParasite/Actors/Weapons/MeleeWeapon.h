// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class PROJECTPARASITE_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TSubclassOf<UDamageType> damageType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float damage = 10;
};
