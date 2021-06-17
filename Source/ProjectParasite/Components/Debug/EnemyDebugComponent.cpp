// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDebugComponent.h"

#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Utilities/DrawDebugHelpersExtended.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Utilities/DevUtils.h"


UEnemyDebugComponent::UEnemyDebugComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	
	enemyRef = Cast<APawnEnemy>(GetOwner());
}

void UEnemyDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FBoxSphereBounds enemyColBounds = enemyRef->GetCollider()->Bounds;

	SCone coneData {
		enemyColBounds.Origin,
		enemyRef->GetActorRotation().Quaternion(),
		enemyRef->GetDetectionAngle(),
		enemyColBounds.BoxExtent.Z * 2,
		enemyRef->GetDetectionRange()
	};

	const TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	const TArray<AActor*> actorsToIgnore { };
	UClass* classFilter = APawnParasite::StaticClass();
	TArray<AActor*> outActors;

	if(OverlapCone(coneData, GetWorld(), objectTypes, classFilter, actorsToIgnore, outActors))
	{
		coneColor = FColor::Green;
	}
	else
	{
		coneColor = FColor::Blue;
	}
	
	DrawCone(coneData, GetWorld(), coneColor);
}
