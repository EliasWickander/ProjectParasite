// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedPawnEnemy.h"

#include "ProjectParasite/Actors/Weapons/RangedWeapon.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"

ARangedPawnEnemy::ARangedPawnEnemy()
{

}

void ARangedPawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetWeaponOfType(weaponType);
}

void ARangedPawnEnemy::Attack()
{
	Super::Attack();

	if(equippedWeapon)
	{
		ARangedWeapon* rangedWeapon = Cast<ARangedWeapon>(equippedWeapon);
		
		if(rangedWeapon)
		{
			//Fire weapon
			equippedWeapon->Use();

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

void ARangedPawnEnemy::UpdatePawnBehavior(float deltaSeconds)
{
	Super::UpdatePawnBehavior(deltaSeconds);
}
