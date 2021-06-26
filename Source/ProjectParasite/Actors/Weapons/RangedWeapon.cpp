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

	currentAmmo = maxAmmo;
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If enemy doesn't have to reload, handle attack timer
	if(reloadTimer <= 0)
	{
		attackTimer = FMath::Clamp<float>(attackTimer - DeltaTime, 0, attackRate);
	}
	else
	{
		//If enemy has to reload, handle reload timer
		reloadTimer = FMath::Clamp<float>(reloadTimer - DeltaTime, 0, timeToReload);
	}
}

void ARangedWeapon::Use()
{
	Super::Use();

	//If out of ammo, don't do anything
	if(currentAmmo <= 0)
	{
		return;
	}
	
	//Only attack if projectile type is set
	if(projectile)
	{
		//If enemy can fire
		if(attackTimer <= 0)
		{
			//Fire from weapon socket
			FVector spawnPos = weaponSocket->GetComponentLocation();
			FRotator spawnRot = weaponSocket->GetComponentRotation();

			AProjectile* spawnedProjectile = GetWorld()->SpawnActor<AProjectile>(projectile, spawnPos, spawnRot);

			//Set reference to this weapon
			spawnedProjectile->weaponRef = this;
	
			//Reset attack timer
			attackTimer = attackRate;

			currentAmmo--;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no projectile type set. Please check that it is set up properly in the blueprint"));
	}
}

void ARangedWeapon::Reload()
{
	reloadTimer = timeToReload;
	currentAmmo = maxAmmo;	
}