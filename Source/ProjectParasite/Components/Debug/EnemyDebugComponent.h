// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyDebugComponent.generated.h"

struct SCone
{
	FVector originPoint;
	FQuat rotation;
	float angle;
	float height;
	float range;
};

struct SConeVertices
{
	FVector originTopPoint;
	FVector originBotPoint;
	FVector topLeftFrontEnd;
	FVector topRightFrontEnd;
	FVector botLeftFrontEnd;
	FVector botRightFrontEnd;
};

struct SCube
{
	FVector center;
	FVector extents;
	FQuat rotation;
};

class APawnEnemy;
UCLASS()
class PROJECTPARASITE_API UEnemyDebugComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UEnemyDebugComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	AActor* debugObject = nullptr;

	FColor coneColor = FColor::Blue;
	FColor boxColor = FColor::Yellow;

private:
	APawnEnemy* enemyRef = nullptr;

	void OverlapCone(SCone cone);

	void DrawCone(SCone cone);
	void DrawConeFromVertices(SConeVertices vertices);

	SConeVertices GetConeVerticesFromData(SCone cone);
	SCube GetCubeContainingCone(SCone cone);
	void DrawCubeContainingCone(SCone cone);
	
	FVector AngleVector(float deg);

};
