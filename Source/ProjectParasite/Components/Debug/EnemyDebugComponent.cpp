// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDebugComponent.h"

#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectParasite/Pawns/PawnParasite.h"


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

	//TODO: Do an overlap box as represented by cone
	//TODO: If player is overlapping box, check its angle to see if within cone
	if(debugObject != nullptr)
	{
		FVector objectPos = debugObject->GetActorLocation();

		FVector dirToObject = objectPos - GetOwner()->GetActorLocation();
		
		if(dirToObject.Size() <= enemyRef->GetDetectionRange())
		{
			coneColor = FColor::Green;
		}
		else
		{
			coneColor = FColor::Blue;
		}
	}
	
	FBoxSphereBounds enemyColBounds = enemyRef->GetCollider()->Bounds;

	SCone coneData {
		enemyColBounds.Origin,
		enemyRef->GetActorRotation().Quaternion(),
		enemyRef->GetDetectionAngle(),
		enemyColBounds.BoxExtent.Z * 2,
		enemyRef->GetDetectionRange()
	};

	OverlapCone(coneData);
	DrawCone(coneData);
	DrawCubeContainingCone(coneData);
	
}

void UEnemyDebugComponent::OverlapCone(SCone cone)
{
	SCube cubeData = GetCubeContainingCone(cone);

	const TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	const TArray<AActor*> actorsToIgnore { };
	UClass* classFilter = Cast<UClass>(enemyRef->playerRef);
	TArray<AActor*> outActors;
	
	bool overlapped = UKismetSystemLibrary::BoxOverlapActors(
		GetWorld(),
		cubeData.center,
		cubeData.extents,
		objectTypes,
		classFilter,
		actorsToIgnore,
		outActors
		);

	// for(AActor* actor : outActors)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Overlapped %s"), *actor->GetName());
	// }
	if(overlapped)
	{
		boxColor = FColor::Green;
	}
	else
	{
		boxColor = FColor::Yellow;
	}
	
}

void UEnemyDebugComponent::DrawCone(SCone cone)
{
	SConeVertices vertices = GetConeVerticesFromData(cone);
	DrawConeFromVertices(vertices);
}

SConeVertices UEnemyDebugComponent::GetConeVerticesFromData(SCone cone)
{
	SConeVertices verticesStruct;

	float halfHeight = cone.height * 0.5f;
	float halfAngle = cone.angle * 0.5f;

	FTransform rotatedTransform(cone.rotation);
	
	verticesStruct.originTopPoint = cone.originPoint + FVector(0, 0, halfHeight);
	verticesStruct.originBotPoint = cone.originPoint - FVector(0, 0, halfHeight);
	
	verticesStruct.topRightFrontEnd = verticesStruct.originTopPoint + rotatedTransform.TransformPosition(AngleVector(halfAngle)) * cone.range;
	verticesStruct.topLeftFrontEnd = verticesStruct.originTopPoint + rotatedTransform.TransformPosition(AngleVector(-halfAngle)) * cone.range;
	verticesStruct.botRightFrontEnd = verticesStruct.originBotPoint + rotatedTransform.TransformPosition(AngleVector(halfAngle)) * cone.range;
	verticesStruct.botLeftFrontEnd = verticesStruct.originBotPoint + rotatedTransform.TransformPosition(AngleVector(-halfAngle)) * cone.range;

	return verticesStruct;
	
}

void UEnemyDebugComponent::DrawConeFromVertices(SConeVertices vertices)
{
	//Draw the four lines making up the cone's shape
	DrawDebugLine(GetWorld(), vertices.originTopPoint, vertices.topRightFrontEnd, coneColor);
	DrawDebugLine(GetWorld(), vertices.originTopPoint, vertices.topLeftFrontEnd, coneColor);
	DrawDebugLine(GetWorld(), vertices.originBotPoint, vertices.botRightFrontEnd, coneColor);
	DrawDebugLine(GetWorld(), vertices.originBotPoint, vertices.botLeftFrontEnd, coneColor);

	//Draw the four lines connecting the four above
	DrawDebugLine(GetWorld(), vertices.botRightFrontEnd, vertices.topRightFrontEnd, coneColor);
	DrawDebugLine(GetWorld(), vertices.botLeftFrontEnd, vertices.topLeftFrontEnd, coneColor);
	DrawDebugLine(GetWorld(), vertices.topLeftFrontEnd, vertices.topRightFrontEnd, coneColor);
	DrawDebugLine(GetWorld(), vertices.botLeftFrontEnd, vertices.botRightFrontEnd, coneColor);
}

SCube UEnemyDebugComponent::GetCubeContainingCone(SCone cone)
{
	SConeVertices coneVertices = GetConeVerticesFromData(cone);
	
	FVector topFrontCenter = (coneVertices.topRightFrontEnd + coneVertices.topLeftFrontEnd) / 2;
	
	FVector centerPoint = (topFrontCenter + coneVertices.originTopPoint) / 2;
	centerPoint.Z = enemyRef->GetActorLocation().Z;
	
	float coneDepth = (topFrontCenter - coneVertices.originTopPoint).Size();
	float coneWidth = (coneVertices.topLeftFrontEnd - coneVertices.topRightFrontEnd).Size();
	float coneHeight = (coneVertices.topLeftFrontEnd - coneVertices.botLeftFrontEnd).Size();
	
	FVector extents(coneDepth / 2, coneWidth / 2,  coneHeight / 2);

	return SCube {centerPoint, extents, cone.rotation};
}

void UEnemyDebugComponent::DrawCubeContainingCone(SCone cone)
{
	SCube cubeData = GetCubeContainingCone(cone);
	
	DrawDebugBox(GetWorld(), cubeData.center, cubeData.extents, cubeData.rotation, boxColor);
}

FVector UEnemyDebugComponent::AngleVector(float deg)
{
	float angleInRad = FMath::DegreesToRadians(deg);

	return FVector(FMath::Cos(angleInRad), FMath::Sin(angleInRad), 0);
}
