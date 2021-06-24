// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class ARangedWeapon;
UCLASS()
class PROJECTPARASITE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
friend class ARangedWeapon;
	
public:
	
	AProjectile();

	virtual void Tick(float DeltaTime) override;

protected:

	void BeginPlay() override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp,
	FVector normalImpulse, const FHitResult& result);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* projectileMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float lifeTime = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float moveSpeed = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<UDamageType> damageType;

	UPROPERTY(BlueprintReadOnly)
	ARangedWeapon* weaponRef = nullptr;

	float lifeTimer = 0;
};
