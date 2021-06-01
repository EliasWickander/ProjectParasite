// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemy.h"
#include "PawnShooter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPARASITE_API APawnShooter : public APawnEnemy
{
	GENERATED_BODY()

public:

	APawnShooter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
