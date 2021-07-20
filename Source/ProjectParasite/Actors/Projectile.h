// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class ARangedWeapon;
class UCapsuleComponent;

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
	void OnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* projectileMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* capsuleComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float lifeTime = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float moveForce = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<UDamageType> damageType;

	UPROPERTY(BlueprintReadOnly)
	ARangedWeapon* weaponRef = nullptr;

	float lifeTimer = 0;
};
