// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnParasite.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AParasitePlayerController;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* camera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* springArm = nullptr;
	
	void CalculateMovementHorizontal(float axis);
	void CalculateMovementVertical(float axis);

	void RotateToMouseCursor();
	void MoveInInputDirection();

	AParasitePlayerController* playerControllerRef;
	
	float horizontalAxis = 0;
	float verticalAxis = 0;
};
