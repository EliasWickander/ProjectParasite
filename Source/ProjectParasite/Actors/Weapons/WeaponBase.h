// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class APawnEnemy;
class USphereComponent;
UCLASS()
class PROJECTPARASITE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:

	friend class APawnEnemy;

	AWeaponBase();

	virtual void Tick(float DeltaTime) override;

	virtual void Trigger();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* baseMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* pickupTrigger = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool isEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackRate = 2;
	float attackTimer = 0;
	
	APawnEnemy* shooterRef = nullptr;
};
