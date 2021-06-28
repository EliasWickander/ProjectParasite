// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyDebugComponent.generated.h"

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

	FColor coneColor = FColor::Blue;

private:
	APawnEnemy* enemyRef = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "General")
	bool debugEnabled = true;
	
	void DrawDetectionCone();

};
