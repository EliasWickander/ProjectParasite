// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemy.h"

#include "ProjectParasite/PlayerControllers/PlayerControllerBase.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Components/Debug/EnemyDebugComponent.h"
#include "AIController.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/DamageTypes/DamageType_Environmental.h"

APawnEnemy::APawnEnemy()
{
	napeComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Nape"));
	napeComponent->SetupAttachment(baseMesh);

	weaponSocket = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Socket"));
	weaponSocket->SetupAttachment(baseMesh);

	enemyDebugger = CreateDefaultSubobject<UEnemyDebugComponent>(TEXT("Debug Component"));
}

void APawnEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	AIController = Cast<AAIControllerBase>(GetController());
}

void APawnEnemy::UpdatePawnBehavior(float deltaSeconds)
{
	Super::UpdatePawnBehavior(deltaSeconds);
	
	if(IsPlayerControlled())
		RotateToMouseCursor();
}

void APawnEnemy::OnDeath(APawnBase* deadPawn, AActor* causerActor)
{
	Super::OnDeath(deadPawn, causerActor);

	//Drop weapon
	SetWeapon(nullptr);

	APawnEnemy* deadEnemy = Cast<APawnEnemy>(deadPawn);

	Cast<AAIControllerBase>(deadEnemy->GetAIController())->SetCurrentState(EnemyStates::State_Idle);
}

void APawnEnemy::SetWeapon(AWeaponBase* newWeapon)
{
	//If there's an old weapon equipped
	if(equippedWeapon)
	{
		equippedWeapon->isEquipped = false;
		equippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	//If new weapon isn't null
	if(newWeapon)
	{
		newWeapon->SetActorLocation(weaponSocket->GetComponentLocation());
		newWeapon->SetActorRotation(weaponSocket->GetComponentRotation());
		newWeapon->AttachToComponent(weaponSocket, FAttachmentTransformRules::KeepWorldTransform);

		newWeapon->weaponHolderRef = this;
		newWeapon->isEquipped = true;	
	} 
	
	equippedWeapon = newWeapon;
}

void APawnEnemy::Attack()
{
	if(equippedWeapon == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no weapon attached. Cannot attack."), *GetName());
	}
}

void APawnEnemy::SetWeaponOfType(TSubclassOf<AWeaponBase> weaponType)
{
	//Spawn weapon
	if(weaponType)
	{
		AWeaponBase* weaponInstance = Cast<AWeaponBase>(GetWorld()->SpawnActor(weaponType));
		
		SetWeapon(weaponInstance);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no weapon type set."), *GetName());
	}
}