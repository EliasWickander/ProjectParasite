// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(weaponHolderRef)
	{
		if(attackingTimer > 0)
		{
			attackingTimer -= DeltaTime;
		}
		else
		{
			weaponHolderRef->SetIsAttacking(false);
			OnStabEnd();
			attackingTimer = 0;
		}	
	}
	
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

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	UClass* classFilter = nullptr;
	TArray<AActor*> actorsToIgnore { };
	TArray<AActor*> outActors;
	
	OverlapCone(coneData, GetWorld(), objectTypes, classFilter, actorsToIgnore, outActors);

	for(int i = 0; i < outActors.Num(); i++)
	{
		AActor* actor = outActors[i];
		//Check if something is obstructing the vision of actors

		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(weaponHolderRef);
		
		if(GetWorld()->LineTraceSingleByChannel(hitResult,weaponHolderRef->GetActorLocation(),actor->GetActorLocation(), ECC_Visibility, params))
		{
			if(hitResult.GetActor())
			{
				//If something is obstructing the vision
				if(hitResult.GetActor() != actor && !Cast<ADestructibleActor>(hitResult.GetActor()))
				{
					outActors.Remove(actor);
					i = 0;
				}	
			}
		}
	}

	return outActors;
}

void AMeleeWeapon::Attack()
{
	TArray<AActor*> hitActors = GetHitActors();

	weaponHolderRef->SetIsAttacking(true);
	attackingTimer = 0.2f;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), swingSound, GetActorLocation(), 1);
	OnStabStart();
	
	if(hitActors.Num() > 0)
	{
		for(AActor* actor : hitActors)
		{
			if(Cast<APawnBase>(actor))
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
			else if(Cast<ADestructibleActor>(actor))
			{
				UDestructibleComponent* destructibleComponent = Cast<ADestructibleActor>(actor)->GetDestructibleComponent();

				FVector impulseDir = destructibleComponent->GetComponentLocation() - GetActorLocation();
				impulseDir.Z = 0;
				impulseDir.Normalize();

				destructibleComponent->ApplyDamage(damage, destructibleComponent->GetComponentLocation(), impulseDir, 10);
			}
		}
	}
}


