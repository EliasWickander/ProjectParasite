// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemyRanged.h"

#include "ProjectParasite/Actors/Weapons/WeaponBase.h"

APawnEnemyRanged::APawnEnemyRanged()
{

}

void APawnEnemyRanged::BeginPlay()
{
	Super::BeginPlay();
}

void APawnEnemyRanged::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APawnEnemyRanged::Attack()
{
	Super::Attack();

	if(equippedWeapon)
	{
		equippedWeapon->Trigger();	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't have a child actor attached matching a weapon. Can't fire."));
	}
}