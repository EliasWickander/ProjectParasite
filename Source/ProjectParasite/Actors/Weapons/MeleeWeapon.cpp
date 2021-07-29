// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Handle attack timer
	attackTimer = FMath::Clamp<float>(attackTimer - DeltaTime, 0, attackRate);
}

void AMeleeWeapon::Use()
{
	Super::Use();

	if(attackTimer <= 0)
	{		
		Attack();

		attackTimer = attackRate;
	}
}

TArray<AActor*> AMeleeWeapon::GetHitActors()
{
	FBoxSphereBounds enemyColBounds = weaponHolderRef->GetCollider()->Bounds;

	SCone coneData
	{
		enemyColBounds.Origin + weaponHolderRef->GetTransform().TransformVector(attackConeOriginOffsetLocal),
		weaponHolderRef->GetActorRotation().Quaternion(),
		attackConeAngle,
		enemyColBounds.BoxExtent.Z * 2,
		GetAttackConeRange()
	};

	UE_LOG(LogTemp, Warning, TEXT("%f"), GetAttackConeRange());

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	UClass* classFilter = APawnBase::StaticClass();
	TArray<AActor*> actorsToIgnore { };
	TArray<AActor*> outActors;
	
	OverlapCone(coneData, GetWorld(), objectTypes, classFilter, actorsToIgnore, outActors);

	return outActors;
}

void AMeleeWeapon::Attack()
{
	TArray<AActor*> hitActors = GetHitActors();
		
	if(hitActors.Num() > 0)
	{
		for(AActor* actor : hitActors)
		{
			
			//Find the first pawn overlapped and apply damage to it
			APawnBase* hitPawn = Cast<APawnBase>(actor);

			APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			//If weapon holder is controlled by player
			if(weaponHolderRef->GetController() == playerController)
			{
				bool hitPawnIsParasite = Cast<APawnParasite>(hitPawn) != nullptr;
				
				//Only hit units that aren't controlled by player
				if(hitPawn->GetController() != playerController && !hitPawnIsParasite)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit %s, %s"), *actor->GetName(), *hitPawn->StaticClass()->GetName());
					UGameplayStatics::ApplyDamage(hitPawn, damage, playerController, weaponHolderRef, damageType);
				}	
			}
			else
			{
				//If weapon holder is not controlled by player
				if(weaponHolderRef->GetController() != playerController)
				{
					//Only hit units that are controlled by player
					if(hitPawn->GetController() == playerController)
					{
						UGameplayStatics::ApplyDamage(hitPawn, damage, playerController, weaponHolderRef, damageType);	
					}
				}
			}
		}
	}
}


