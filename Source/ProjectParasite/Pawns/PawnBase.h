// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/DefaultPawn.h"
#include "GameFramework/Pawn.h"

#include "PawnBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UMovementComponent;
class AParasitePlayerController;
UCLASS()
class PROJECTPARASITE_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnBase();

	virtual void Tick(float DeltaSeconds) override;

	//Casts a ray to mouse cursor in world. Returns true if hit found
	bool RaycastToMouseCursor(FHitResult& hitResult);

	void SetCanMove(bool enabled) { canMove = enabled; }
	bool GetCanMove() { return canMove; }

	AParasitePlayerController* playerControllerRef = nullptr;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* capsuleCollider = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* baseMesh = nullptr;
	
	float horizontalAxis = 0;
	float verticalAxis = 0;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void Move(FVector moveDir);
	void Rotate(FVector targetPoint);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* camera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* springArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent* movementComponent = nullptr;
    	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float moveSpeed = 300;

	bool canMove = true;
	
	void MoveHorizontal(float axis);
	void MoveVertical(float axis);

	void RotateToMouseCursor();

};
