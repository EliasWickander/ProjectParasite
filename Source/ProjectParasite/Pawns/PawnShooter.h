// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemyRanged.h"

#include "PawnShooter.generated.h"

UCLASS()
class PROJECTPARASITE_API APawnShooter : public APawnEnemyRanged
{
	GENERATED_BODY()

public:

	APawnShooter();

	virtual void UpdatePawnBehavior(float deltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;

};
