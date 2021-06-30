// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "PawnBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UFloatingPawnMovement;
class APawnBase;
class APlayerControllerBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathEvent, APawnBase*, deadPawn, AActor*, causerActor);

UCLASS()
class PROJECTPARASITE_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	APawnBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void UpdatePawnBehavior(float deltaSeconds);
	
	//Casts a ray to mouse cursor in world. Returns true if hit found
	bool RaycastToMouseCursor(FHitResult& hitResult);
	void RotateToMouseCursor();

	UCapsuleComponent* GetCollider() { return capsuleCollider; }
	
	void SetMoveSpeed(float speed);
	float GetMoveSpeed() { return currentMoveSpeed; }
	
	void SetCanMove(bool enabled) { canMove = enabled; }
	bool GetCanMove() { return canMove; }

	bool GetIsPendingDeath() { return isPendingDeath; }

	float GetMaxHealth() { return maxHealth; }
	float GetCurrentHealth() { return currentHealth; }

	virtual void HandlePendingDeath();

	UFUNCTION()
	virtual void OnDeath(APawnBase* deadPawn, AActor* causerActor);
	
	UFUNCTION()
	void OnTakeDamage(AActor* damagedActor, float damage, const UDamageType* damageType, AController* causerController, AActor* causerActor);
	
	void MoveHorizontal(float axis);
	void MoveVertical(float axis);

	FOnDeathEvent onStartDeathEvent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent* movementComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float maxHealth = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float currentHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float currentMoveSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float deathTime = 1;
	float deathTimer = 0;
	
	bool canMove = true;
	
	UFloatingPawnMovement* floatingPawnMovement = nullptr;
};
