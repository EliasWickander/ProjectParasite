// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "PawnBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UMovementComponent;
class UFloatingPawnMovement;
class APlayerControllerBase;

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

	virtual void Tick(float DeltaSeconds) override;

	virtual void UpdatePawnBehavior(float deltaSeconds);

	virtual void HandleDeath();

	UFUNCTION()
	void OnTakeDamage(AActor* damagedActor, float damage, const UDamageType* damageType, AController* causerController, AActor* causerActor);

	virtual void OnStartDeath(AActor* pawnBeingDestroyed);
	void MoveHorizontal(float axis);
	void MoveVertical(float axis);
	
	UCapsuleComponent* GetCollider() { return capsuleCollider; }
    
    APlayerControllerBase* playerControllerRef = nullptr;
    	
protected:
	virtual void BeginPlay() override;
	
	void Rotate(FVector targetPoint);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* capsuleCollider = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* baseMesh = nullptr;
	
	float horizontalAxis = 0;
	float verticalAxis = 0;
	
	bool isPendingDeath = false;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* camera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* springArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float maxHealth = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float currentMoveSpeed = 0;
	
	float currentHealth;
	
	bool canMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent* movementComponent = nullptr;
	
	UFloatingPawnMovement* floatingPawnMovement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float deathTime = 1;
	float deathTimer = 0;
};
