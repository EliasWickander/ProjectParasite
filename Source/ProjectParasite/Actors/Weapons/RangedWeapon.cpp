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

}

void ARangedWeapon::Trigger()
{
	Super::Trigger();

	//Only attack if projectile type is set
	if(projectile)
	{
		//If pawn can fire
		if(attackTimer == 0)
		{
			//Fire from weapon socket
			FVector spawnPos = weaponSocket->GetComponentLocation();
			FRotator spawnRot = weaponSocket->GetComponentRotation();

			AProjectile* spawnedProjectile = GetWorld()->SpawnActor<AProjectile>(projectile, spawnPos, spawnRot);

			//Set reference to this weapon
			spawnedProjectile->weaponRef = this;
	
			//Reset attack timer
			attackTimer = attackRate;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no projectile type set. Please check that it is set up properly in the blueprint"));
	}
}

