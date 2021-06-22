// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class APawnParasite;
class USphereComponent;
UCLASS()
class PROJECTPARASITE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:

	friend class APawnEnemy;
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Trigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* baseMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* pickupTrigger = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
	bool isPickedUp = false;

	APawnParasite* playerRef = nullptr;

};
