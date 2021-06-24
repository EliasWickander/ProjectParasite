// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PawnBase.h"
#include "PawnEnemy.generated.h"

class ATargetPoint;
class AAIController;
class APawnParasite;
class UEnemyDebugComponent;
class AWeaponBase;

UCLASS()
class PROJECTPARASITE_API APawnEnemy : public APawnBase
{
	GENERATED_BODY()

public:

	friend class UEnemyDebugComponent;
	friend class UBTTask_PatrolBetweenWaypoints;
	friend class AWeaponBase;
	
	APawnEnemy();

	virtual void UpdatePawnBehavior(float deltaSeconds) override;

	virtual void Attack();

	USceneComponent* GetNapeComponent() { return napeComponent; }
	TArray<ATargetPoint*> GetPatrolPoints() { return patrolPoints; }
	AAIController* GetAIController() { return AIController; }
	APawnParasite* GetPlayerRef() { return playerRef; }

	float GetPatrolSpeed() { return patrolSpeed; }
	float GetChaseSpeed() { return chaseSpeed; }

	virtual void OnStartDeath(AActor* pawnBeingDestroyed) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void SetWeapon(AWeaponBase* newWeapon);
	
	float GetAttackRange() { return attackRange; }
	float GetDetectionRange() { return detectionRange; }
	float GetDetectionAngle() { return detectionAngle; }
	float GetSightReactionTime() { return sightReactionTime; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UEnemyDebugComponent* enemyDebugger = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* napeComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<AWeaponBase> weaponType;
	
	AWeaponBase* equippedWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* weaponSocket = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<ATargetPoint*> patrolPoints;
	
	APawnParasite* playerRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float patrolSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float chaseSpeed = 200;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float detectionRange = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float detectionAngle = 60;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float attackRange = 300;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detection")
	float sightReactionTime = 0.2f;

private:
	AAIController* AIController = nullptr;
};
