// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemy.h"

#include "ProjectParasite/PlayerControllers/PlayerControllerBase.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Components/Debug/EnemyDebugComponent.h"
#include "AIController.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "EngineUtils.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"

APawnEnemy::APawnEnemy()
{
	napeComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Nape"));
	napeComponent->SetupAttachment(baseMesh);

	weaponSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	weaponSocket->SetupAttachment(baseMesh);

	//cant move this for some reason
	
	enemyDebugger = CreateDefaultSubobject<UEnemyDebugComponent>(TEXT("Debug Component"));
}

void APawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(weaponType)
	{
		AWeaponBase* weaponInstance = Cast<AWeaponBase>(GetWorld()->SpawnActor(weaponType));
		
		SetWeapon(weaponInstance);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no weapon type set."), *GetName());
	}
	
	playerRef = FindPlayerInWorld();
	
	AIController = Cast<AAIController>(GetController());
}

void APawnEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsPlayerControlled())
		RotateToMouseCursor();

}

void APawnEnemy::SetWeapon(AWeaponBase* newWeapon)
{
	if(equippedWeapon)
	{
		equippedWeapon->isPickedUp = false;
		equippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	newWeapon->SetActorLocation(weaponSocket->GetComponentLocation());
	newWeapon->SetActorRotation(weaponSocket->GetComponentRotation());
	newWeapon->AttachToComponent(weaponSocket, FAttachmentTransformRules::KeepWorldTransform);
	
	newWeapon->isPickedUp = true;
	
	equippedWeapon = newWeapon;
}

void APawnEnemy::SetPossessed(bool enabled)
{
	if(enabled)
	{
		playerControllerRef->Possess(this);
	}
	else
	{
		AIController->Possess(this);
		playerControllerRef->Possess(playerRef);
	}
}

void APawnEnemy::Attack()
{
	if(equippedWeapon == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no weapon attached. Cannot attack."), *GetName());
		return;
	}
}

APawnParasite* APawnEnemy::FindPlayerInWorld()
{
	//Find all parasites in world (should only be one)
	TArray<APawnParasite*> playersInWorld;
	
	for (TActorIterator<APawnParasite> p(GetWorld()); p; ++p)
	{
		APawnParasite* parasiteFound = *p;
		playersInWorld.Add(parasiteFound);
	}

	if(playersInWorld.Num() > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("More than one parasite found in world."))
	}

	//Crash game if no parasite is in world since that would break enemy behaviour
	if(playersInWorld.Num() <= 0)
	{
		UE_LOG(LogTemp, Fatal, TEXT("No parasite found in world."))
	}

	return playersInWorld[0];
}