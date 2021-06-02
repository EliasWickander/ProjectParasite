// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnEnemy.generated.h"

class APawnParasite;

/**
 * 
 */
UCLASS()
class PROJECTPARASITE_API APawnEnemy : public APawnBase
{
	GENERATED_BODY()

public:

	APawnEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
protected:

	APawnParasite* playerRef = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Unpossess();
};
