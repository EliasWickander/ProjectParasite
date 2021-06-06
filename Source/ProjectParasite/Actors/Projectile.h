// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UCapsuleComponent;
UCLASS()
class PROJECTPARASITE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp,
	FVector normalImpulse, const FHitResult& result);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* projectileMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float lifeTime = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float moveSpeed = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TSubclassOf<UDamageType> damageType;

	float timer = 0;
};
