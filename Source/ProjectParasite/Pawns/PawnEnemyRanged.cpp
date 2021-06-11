// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemyRanged.h"

#include "ProjectParasite/Actors/Weapon.h"

APawnEnemyRanged::APawnEnemyRanged()
{

}

void APawnEnemyRanged::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> childActors;

	GetAllChildActors(childActors);

	for(AActor* actor : childActors)
	{
		if(Cast<AWeapon>(actor) != nullptr)
			weapon = Cast<AWeapon>(actor);
	}
}

void APawnEnemyRanged::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APawnEnemyRanged::Attack()
{
	Super::Attack();

	if(weapon)
	{
		weapon->Fire();	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't have a child actor attached matching a weapon. Can't fire."));
	}
}