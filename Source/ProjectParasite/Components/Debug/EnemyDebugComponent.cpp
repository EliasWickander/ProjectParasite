// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDebugComponent.h"

#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"


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
	
	DrawVisionCone();
}

void UEnemyDebugComponent::DrawVisionCone()
{
	FBoxSphereBounds enemyColBounds = enemyRef->GetCollider()->Bounds;

	SCone coneData {
		enemyColBounds.Origin,
		enemyRef->GetDetectionAngle(),
		enemyColBounds.BoxExtent.Z * 2,
		enemyRef->GetDetectionRange()
	};
	
	DrawCone(coneData, enemyRef);
	DrawBoxContainingCone(coneData);
}

void UEnemyDebugComponent::DrawCone(SCone data, AActor* actorRelative)
{
	float halfAngle = data.angle * 0.5f;
	float halfHeight = data.height * 0.5f;
	
	float originAngle = 0;

	if(actorRelative != nullptr)
	{
		originAngle = FVector::DotProduct(FVector::ForwardVector, actorRelative->GetActorForwardVector());

		originAngle = FMath::RadiansToDegrees(FMath::Acos(originAngle));

		FVector cross = FVector::CrossProduct(FVector::ForwardVector, actorRelative->GetActorForwardVector());

		if(cross.Z < 0)
			originAngle = -originAngle;
	}

	SConeVertices vertices = GetConeVerticesFromData(data, originAngle);
	DrawConeFromVertices(vertices);
}

SConeVertices UEnemyDebugComponent::GetConeVerticesFromData(SCone data, float originAngle)
{
	SConeVertices verticesStruct;

	float halfHeight = data.height * 0.5f;
	float halfAngle = data.angle * 0.5f;
	
	verticesStruct.originTopPoint = data.originPoint + FVector(0, 0, halfHeight);
	verticesStruct.originBotPoint = data.originPoint - FVector(0, 0, halfHeight);
	
	verticesStruct.topRightFrontEnd = verticesStruct.originTopPoint + AngleVector(originAngle + halfAngle) * data.range;
	verticesStruct.topLeftFrontEnd = verticesStruct.originTopPoint + AngleVector(originAngle - halfAngle) * data.range;
	verticesStruct.botRightFrontEnd = verticesStruct.originBotPoint + AngleVector(originAngle + halfAngle) * data.range;
	verticesStruct.botLeftFrontEnd = verticesStruct.originBotPoint + AngleVector(originAngle - halfAngle) * data.range;

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

void UEnemyDebugComponent::DrawBoxContainingCone(SCone cone)
{
	SConeVertices coneVertices = GetConeVerticesFromData(cone);
	
	FVector topFrontCenter = (coneVertices.topRightFrontEnd + coneVertices.topLeftFrontEnd) / 2;
	
	FVector centerPoint = (topFrontCenter + coneVertices.originTopPoint) / 2;
	centerPoint.Z = enemyRef->GetActorLocation().Z;
	
	float coneDepth = (topFrontCenter - coneVertices.originTopPoint).Size();
	float coneWidth = (coneVertices.topLeftFrontEnd - coneVertices.topRightFrontEnd).Size();
	float coneHeight = (coneVertices.topLeftFrontEnd - coneVertices.botLeftFrontEnd).Size();
	
	FVector extents(coneDepth / 2, coneWidth / 2,  coneHeight / 2);
	
	DrawDebugBox(GetWorld(), centerPoint, extents, FColor::Yellow);
}

FVector UEnemyDebugComponent::AngleVector(float deg)
{
	float angleInRad = FMath::DegreesToRadians(deg);

	return FVector(FMath::Cos(angleInRad), FMath::Sin(angleInRad), 0);
}
