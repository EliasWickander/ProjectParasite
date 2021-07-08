// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemyDebugComponent.h"

#include "Components/CapsuleComponent.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "ProjectParasite/Actors/Weapons/MeleeWeapon.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Utilities/DrawDebugHelpersExtended.h"

void UMeleeEnemyDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!debugEnabled)
		return;
	
	DrawAttackCone();
}

void UMeleeEnemyDebugComponent::DrawAttackCone()
{
	if(enemyRef)
	{
		AMeleeWeapon* meleeWeaponRef = Cast<AMeleeWeapon>(enemyRef->GetWeapon());

		if(meleeWeaponRef)
		{
			FBoxSphereBounds enemyColBounds = enemyRef->GetCollider()->Bounds;

			SCone coneData
			{
				enemyColBounds.Origin + enemyRef->GetTransform().TransformVector(meleeWeaponRef->GetAttackConeOffset()),
				enemyRef->GetActorRotation().Quaternion(),
				meleeWeaponRef->GetAttackConeAngle(),
				enemyColBounds.BoxExtent.Z * 2,
				meleeWeaponRef->GetAttackConeRange()
			};

			DrawCone(coneData, GetWorld(), FColor::Red);		
		}
	}
}
