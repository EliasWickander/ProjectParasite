// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PawnBase.h"
#include "PawnEnemy.generated.h"

class AAIController;
class APawnParasite;
class UWeapon;

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

	virtual void Attack();

	virtual void SetPossessed(bool isPossessed);
	FVector GetNapeLocation() { return napeComponent->GetComponentLocation(); }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APawnParasite* playerRef = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* napeComponent = nullptr;

private:
	void Unpossess();

	AAIController* AIController = nullptr;
	
};
