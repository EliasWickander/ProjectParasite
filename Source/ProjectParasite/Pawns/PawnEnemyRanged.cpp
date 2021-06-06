// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemyRanged.h"

#include "ProjectParasite/Actors/Projectile.h"

APawnEnemyRanged::APawnEnemyRanged()
{
	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Base Mesh"));
	weaponMesh->SetupAttachment(RootComponent);
	
	weaponSocket = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Socket"));
	weaponSocket->SetupAttachment(weaponMesh);
}

void APawnEnemyRanged::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	attackTimer = FMath::Clamp<float>(attackTimer - DeltaSeconds, 0, fireRate);
}

void APawnEnemyRanged::Attack()
{
	Super::Attack();

	if(projectile && attackTimer == 0)
	{
		FVector spawnPos = weaponSocket->GetComponentLocation();
		FRotator spawnRot = weaponSocket->GetComponentRotation();

		AProjectile* spawnedProjectile = GetWorld()->SpawnActor<AProjectile>(projectile, spawnPos, spawnRot);
		spawnedProjectile->SetOwner(this);

		attackTimer = fireRate;
	}
	
}