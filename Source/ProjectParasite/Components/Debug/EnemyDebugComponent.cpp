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

	if(OverlapCone(coneData, objectTypes, classFilter, actorsToIgnore, outActors))
	{
		coneColor = FColor::Green;
	}
	else
	{
		coneColor = FColor::Blue;
	}
	
	DrawCone(coneData, GetWorld(), coneColor);
}

bool UEnemyDebugComponent::OverlapCone(SCone cone, const TArray<TEnumAsByte<EObjectTypeQuery>>& objectTypes, UClass* actorClassFilter, const TArray<AActor*>& actorsToIgnore, TArray<AActor*>& outActors)
{
	outActors.Empty();
	
	SCube cubeData = GetCubeContainingCone(cone);

	//Check if box is overlapping player
	TArray<AActor*> overlappedActors;
	
	UKismetSystemLibrary::BoxOverlapActors(
		GetWorld(),
		cubeData.center,
		cubeData.extents,
		objectTypes,
		actorClassFilter,
		actorsToIgnore,
		overlappedActors
		);

	//Find the angle to each overlapped actors position
	for(AActor* overlappedActor : overlappedActors)
	{
		FVector dirToActor = overlappedActor->GetActorLocation() - cone.originPoint;
		dirToActor.Z = 0;
		dirToActor.Normalize();

		float angle = AngleBetweenVectors(enemyRef->GetActorForwardVector(), dirToActor);

		//If players angle matches the cone's angle, it is considered inside of the cone
		if(angle <= cone.angle * 0.5f)
		{
			outActors.Push(overlappedActor);
		}
	}

	//Returns true if cone overlaps at least one actor
	return outActors.Num() > 0;
}
