// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"

#include "ProjectParasite/Actors/Projectile.h"

// Sets default values
ARangedWeapon::ARangedWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	weaponSocket = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Socket"));
	weaponSocket->SetupAttachment(baseMesh);

}

// Called when the game starts or when spawned
void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Handle attack timer
	attackTimer = FMath::Clamp<float>(attackTimer - DeltaTime, 0, fireRate);

}

void ARangedWeapon::Trigger()
{
	Super::Trigger();

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

