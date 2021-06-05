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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* projectileMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float lifeTime = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float moveSpeed = 2;

	float timer = 0;
};
