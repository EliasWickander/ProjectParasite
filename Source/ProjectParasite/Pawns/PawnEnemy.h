// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PawnBase.h"
#include "PawnEnemy.generated.h"

class ATargetPoint;
class AAIController;
class APawnParasite;
class UWeapon;
UCLASS()
class PROJECTPARASITE_API APawnEnemy : public APawnBase
{
	GENERATED_BODY()

public:

	APawnEnemy();

	virtual void Tick(float DeltaTime) override;

	void SetPossessed(bool enabled);
	virtual void Attack();
	
	USceneComponent* GetNapeComponent() { return napeComponent; }
	TArray<ATargetPoint*> GetPatrolPoints() { return patrolPoints; }
	AAIController* GetAIController() { return AIController; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* napeComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<ATargetPoint*> patrolPoints;
	
	APawnParasite* playerRef = nullptr;

private:
	APawnParasite* FindPlayerInWorld();

	AAIController* AIController = nullptr;
};
