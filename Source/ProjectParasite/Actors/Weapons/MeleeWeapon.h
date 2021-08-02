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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Use() override;

	TArray<AActor*> GetHitActors();

	void SetAttackConeRange(float range) { attackConeRange = range; }
	float GetAttackConeRange() { return attackConeRange; }
	float GetAttackConeAngle() { return attackConeAngle; }
	FVector GetAttackConeOffset() { return attackConeOriginOffsetLocal; }
	
protected:
	virtual void Attack();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TSubclassOf<UDamageType> damageType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float attackConeRange = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float attackConeAngle = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector attackConeOriginOffsetLocal = FVector(0, 0, -50);

	float attackingTimer = 0;
};
