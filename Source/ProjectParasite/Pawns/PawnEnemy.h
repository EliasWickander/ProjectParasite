// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PawnBase.h"
#include "PawnEnemy.generated.h"

class ATargetPoint;
class AAIControllerBase;
class APawnParasite;
class UPawnSensingComponent;

UCLASS()
class PROJECTPARASITE_API APawnEnemy : public APawnBase
{
	GENERATED_BODY()

friend class UEnemyDebugComponent;
friend class UBTTask_PatrolBetweenWaypoints;
friend class AWeaponBase;
friend class APawnParasite;
	
public:
	
	APawnEnemy();

	virtual void UpdatePawnBehavior(float deltaSeconds) override;

	virtual void Attack();

	virtual void OnDeath(APawnBase* deadPawn, AActor* causerActor) override;

	virtual void OnTakeDamage(AActor* damagedActor, float damage, const UDamageType* damageType, AController* causerController, AActor* causerActor) override;

	UFUNCTION()
	void OnHearNoise(APawn* pawnInstigator, const FVector& location, float volume);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void SetWeapon(AWeaponBase* newWeapon);
	AWeaponBase* GetWeapon() { return equippedWeapon; }

	float GetAttackRange() { return attackRange; }
	float GetDetectionRange() { return detectionRange; }
	float GetDetectionAngle() { return detectionAngle; }
	float GetSightReactionTime() { return sightReactionTime; }
	float GetNoiseReactionTime() { return noiseReactionTime; }

	float GetPatrolSpeed() { return patrolSpeed; }
	float GetChaseSpeed() { return chaseSpeed; }
	float GetPossessedSpeed() { return possessedSpeed; }
	float GetAttackMoveSpeed() { return attackMoveSpeed; }
	float GetTurnRate() { return turnRate; }
	float GetAttackRate() { return attackRate; }

	void SetIsAttacking(bool enabled) { isAttacking = enabled; }
	float GetIsAttacking() { return isAttacking; }

	AAIControllerBase* GetAIController() { return enemyAIController; }
	
	USceneComponent* GetNapeComponent() { return napeComponent; }
	UPawnSensingComponent* GetPawnSensingComponent() { return pawnSensingComponent; }
	TArray<ATargetPoint*> GetPatrolPoints() { return patrolPoints; }
	TArray<ATargetPoint*> GetGuardPoints() { return guardPoints; }
	float GetGuardTime() { return guardTime; }
	float GetGuardTimeOffset() { return guardTimeOffset; }
	bool GetIsStationary() { return isStationary; }
	FVector GetLastHeardNoisePos() { return lastHeardNoisePos; }
	
protected:
	virtual void BeginPlay() override;

	void SetWeaponOfType(TSubclassOf<AWeaponBase> weaponType);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* napeComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* weaponSocket = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPawnSensingComponent* pawnSensingComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AWeaponBase* equippedWeapon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<ATargetPoint*> patrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<ATargetPoint*> guardPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool isStationary = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float guardTime = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float guardTimeOffset = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float attackRate = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float patrolSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float chaseSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float attackMoveSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float possessedSpeed = 150;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float turnRate = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float detectionRange = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float detectionAngle = 60;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float attackRange = 300;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detection")
	float sightReactionTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detection")
	float noiseReactionTime = 0.4f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool isPossessed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool isAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AAIControllerBase* enemyAIController = nullptr;

	FVector lastHeardNoisePos;
};
