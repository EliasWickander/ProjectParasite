// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangedPawnEnemy.h"

#include "PawnShooter.generated.h"

UCLASS()
class PROJECTPARASITE_API APawnShooter : public ARangedPawnEnemy
{
	GENERATED_BODY()

public:

	APawnShooter();

	virtual void UpdatePawnBehavior(float deltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;

};
