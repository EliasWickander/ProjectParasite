// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "PawnBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UHealthComponent;
class UCapsuleComponent;
class UMovementComponent;
class UFloatingPawnMovement;
class APlayerControllerParasite;
UCLASS()
class PROJECTPARASITE_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	APawnBase();

	//Casts a ray to mouse cursor in world. Returns true if hit found
	bool RaycastToMouseCursor(FHitResult& hitResult);
	void RotateToMouseCursor();

	void SetMoveSpeed(float speed);
	float GetMoveSpeed();
	
	void SetCanMove(bool enabled);
	bool GetCanMove();
	
	UCapsuleComponent* GetCollider() { return capsuleCollider; }
	
	APlayerControllerParasite* playerControllerRef = nullptr;
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Rotate(FVector targetPoint);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* capsuleCollider = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* baseMesh = nullptr;
	
	float horizontalAxis = 0;
	float verticalAxis = 0;

private:
	void MoveHorizontal(float axis);
	void MoveVertical(float axis);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* camera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* springArm = nullptr;
	
	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	 UHealthComponent* healthComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float moveSpeed = 300;
	
	bool canMove = true;

	UPROPERTY()
	UPawnMovementComponent* movementComponent = nullptr;
	UFloatingPawnMovement* floatingPawnMovement = nullptr;
};
