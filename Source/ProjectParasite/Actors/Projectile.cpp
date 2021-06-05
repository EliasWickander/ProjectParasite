// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	
	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	projectileMesh->SetupAttachment(RootComponent);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(FVector::ForwardVector * moveSpeed);
	
	if(timer < lifeTime)
	{
		timer += DeltaTime;
	}
	else
	{
		Destroy();
	}
}

