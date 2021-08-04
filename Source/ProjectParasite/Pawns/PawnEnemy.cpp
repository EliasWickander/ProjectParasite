// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemy.h"

#include "ProjectParasite/PlayerControllers/PlayerControllerBase.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "AIController.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/DamageTypes/DamageType_Environmental.h"
#include "Perception/PawnSensingComponent.h"
#include "ProjectParasite/Actors/Weapons/MeleeWeapon.h"
#include "ProjectParasite/Actors/Weapons/RangedWeapon.h"
#include "ProjectParasite/Utilities/StateMachine/StateMachine.h"

APawnEnemy::APawnEnemy()
{
	napeComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Nape"));
	napeComponent->SetupAttachment(skeletalMesh);

	weaponSocket = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Socket"));
	weaponSocket->SetupAttachment(skeletalMesh);

	pawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
}

void APawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	//AIController = Cast<AAIControllerBase>(GetController());
	enemyAIController = GetWorld()->SpawnActor<AAIControllerBase>(AIControllerClass, GetActorLocation(), GetActorRotation());
	
	if(!Cast<APlayerControllerBase>(GetController()))
	{
		enemyAIController->Possess(this);
	}

	pawnSensingComponent->OnHearNoise.AddDynamic(this, &APawnEnemy::OnHearNoise);
}

void APawnEnemy::RotateWeaponToMouseCursor()
{

	if(equippedWeapon)
	{
		const USkeletalMeshSocket* socket;
		
		if(Cast<ARangedWeapon>(equippedWeapon))
		{
			socket = skeletalMesh->GetSocketByName("Gun_ExpSocket");	
		}
		else
		{
			socket = skeletalMesh->GetSocketByName("Knife_ExpSocket");	
		}

		FHitResult hitResult;

		//If the ray-cast to cursor hit something, rotate to hit location
		if(RaycastToMouseCursor(hitResult))
		{
			//Get dir to rotate in
			hitResult.Location.Z = GetActorLocation().Z;
	
			FVector rotationDir = hitResult.Location - socket->GetSocketLocation(skeletalMesh);

			if(rotationDir.Size() <= 200)
				return;
			
			rotationDir.Normalize();

			//Rotate
			FRotator targetRotation = rotationDir.Rotation();

			equippedWeapon->SetActorRotation(targetRotation);
		}
	}
}

void APawnEnemy::UpdatePawnBehavior(float deltaSeconds)
{
	Super::UpdatePawnBehavior(deltaSeconds);
	
	if(IsPlayerControlled())
	{
		RotateToMouseCursor();	
		RotateWeaponToMouseCursor();
		
		if(GetActorLocation().Z != GetStartPos().Z)
		{
			FVector targetPos = GetActorLocation();
			targetPos.Z = GetStartPos().Z;

			if(FVector::Dist(GetActorLocation(), targetPos) > 0.05f)
			{
				FVector currentLocation = FMath::Lerp(GetActorLocation(), targetPos, 5 * GetWorld()->GetDeltaSeconds());

				SetActorLocation(currentLocation, true);
			}
		}
	}
}

void APawnEnemy::OnDeath(APawnBase* deadPawn, AActor* causerActor)
{
	Super::OnDeath(deadPawn, causerActor);

	APawnEnemy* deadEnemy = Cast<APawnEnemy>(deadPawn);

	Cast<AAIControllerBase>(deadEnemy->GetAIController())->StopAIBehavior();

	capsuleCollider->DestroyComponent();
	//Drop weapon
	SetWeapon(nullptr);

	if(Cast<APawnEnemy>(causerActor) && playerControllerRef->GetPlayer()->GetPossessedEnemy() == this)
	{
		playerControllerRef->GetPlayer()->GetStateMachine()->SetState("State_Idle");
	}
}

void APawnEnemy::OnTakeDamage(AActor* damagedActor, float damage, const UDamageType* damageType,
	AController* causerController, AActor* causerActor)
{
	Super::OnTakeDamage(damagedActor, damage, damageType, causerController, causerActor);
}

void APawnEnemy::OnHearNoise(APawn* pawnInstigator, const FVector& location, float volume)
{
	if(pawnInstigator != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Heard noise from %s"), *pawnInstigator->GetName());

		lastHeardNoisePos = location;

		EnemyStates currentState = GetAIController()->GetCurrentState();
		
		if(currentState == EnemyStates::State_Attack || currentState == EnemyStates::State_Stunned || currentState == EnemyStates::State_Chase)
			return;

		GetAIController()->SetCurrentState(EnemyStates::State_NoiseSearch);
	}
}


void APawnEnemy::SetWeapon(AWeaponBase* newWeapon)
{
	//If there's an old weapon equipped
	if(equippedWeapon)
	{
		equippedWeapon->weaponHolderRef = nullptr;
		equippedWeapon->isEquipped = false;
		equippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	//If new weapon isn't null
	if(newWeapon)
	{
		const USkeletalMeshSocket* socket;

		if(Cast<ARangedWeapon>(newWeapon))
		{
			skeletalMesh->SetAnimClass(rangedAnimBlueprint);
			socket = skeletalMesh->GetSocketByName("Gun_ExpSocket");
		}
		else
		{
			skeletalMesh->SetAnimClass(meleeAnimBlueprint);
			socket = skeletalMesh->GetSocketByName("Knife_ExpSocket");
		}

		if(socket)
		{
			socket->AttachActor(newWeapon, skeletalMesh);	
		}
		
		// newWeapon->SetActorLocation(weaponSocket->GetComponentLocation());
		// newWeapon->SetActorRotation(weaponSocket->GetComponentRotation());
		// newWeapon->AttachToComponent(weaponSocket, FAttachmentTransformRules::KeepWorldTransform);

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