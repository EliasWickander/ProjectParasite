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

	//Handle attack timer
	attackTimer = FMath::Clamp<float>(attackTimer - DeltaSeconds, 0, fireRate);
}

void APawnEnemyRanged::Attack()
{
	Super::Attack();

	//Only attack if projectile type is set
	if(projectile)
	{
		//If pawn can attack
		if(attackTimer == 0)
		{
			FVector spawnPos = weaponSocket->GetComponentLocation();
			FRotator spawnRot = weaponSocket->GetComponentRotation();

			AProjectile* spawnedProjectile = GetWorld()->SpawnActor<AProjectile>(projectile, spawnPos, spawnRot);

			//Set projectile's owner to this pawn (used in projectile as a reference to this pawn)
			spawnedProjectile->SetOwner(this);

			//Reset attack timer
			attackTimer = fireRate;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no projectile type set. Please check that it is set up properly in the blueprint"));
	}
	
}