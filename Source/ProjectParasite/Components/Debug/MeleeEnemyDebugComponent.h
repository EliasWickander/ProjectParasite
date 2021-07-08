// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyDebugComponent.h"
#include "MeleeEnemyDebugComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPARASITE_API UMeleeEnemyDebugComponent : public UEnemyDebugComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	void DrawAttackCone();
};
