// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleePawnEnemy.h"

#include "ProjectParasite/Actors/Weapons/MeleeWeapon.h"

void AMeleePawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetWeaponOfType(weaponType);
}

void AMeleePawnEnemy::Attack()
{
	Super::Attack();

	if(equippedWeapon)
	{
		AMeleeWeapon* meleeWeapon = Cast<AMeleeWeapon>(equippedWeapon);
		
		if(meleeWeapon)
		{
			//Fire weapon
			equippedWeapon->Use();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s has a ranged weapon regardless of being a melee enemy"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't have a child actor attached matching a weapon. Can't fire."));
	}
}
