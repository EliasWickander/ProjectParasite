// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnParasite.generated.h"


class UParasiteDebugComponent;
class APawnEnemy;
class UStateMachine;
class UParasiteStateMachine;

/**
 * 
 */
UCLASS()
class PROJECTPARASITE_API APawnParasite : public APawnBase
{
	GENERATED_BODY()

public:

	APawnParasite();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetPossessRadius() { return possessRadius; }

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParasiteDebugComponent* parasiteDebugger = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParasiteStateMachine* stateMachine = nullptr;

	void PossessClosestEnemyInRadius();
	void Dash();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float possessRadius = 100;
};
