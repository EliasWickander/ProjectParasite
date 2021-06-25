// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemyRanged.h"

#include "ProjectParasite/Actors/Weapons/RangedWeapon.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"

APawnEnemyRanged::APawnEnemyRanged()
{

}

void APawnEnemyRanged::BeginPlay()
{
	Super::BeginPlay();
}

void APawnEnemyRanged::Attack()
{
	Super::Attack();

	if(equippedWeapon)
	{
		ARangedWeapon* rangedWeapon = Cast<ARangedWeapon>(equippedWeapon);
		
		if(rangedWeapon)
		{
			//Fire weapon
			equippedWeapon->Trigger();

			//If no ammo, reload weapon
			if(rangedWeapon->GetCurrentAmmo() <= 0)
			{
				rangedWeapon->Reload();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s has a non-ranged weapon regardless of being a ranged enemy"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't have a child actor attached matching a weapon. Can't fire."));
	}
}

void APawnEnemyRanged::UpdatePawnBehavior(float deltaSeconds)
{
	Super::UpdatePawnBehavior(deltaSeconds);
}
