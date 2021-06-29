// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

UENUM(BlueprintType)
enum class EnemyStates : uint8
{
	State_Idle UMETA(DisplayName = "Idle"),
	State_Patrol UMETA(DisplayName = "Patrol"),
	State_Chase UMETA(DisplayName = "Chase"),
	State_Attack UMETA(DisplayName = "Attack"),
	State_Stunned UMETA(DisplayName = "Stunned")
};

class APawnParasite;
class APawnEnemy;
UCLASS()
class PROJECTPARASITE_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	void SetCurrentState(EnemyStates state);
	EnemyStates GetCurrentState() { return currentState; }

	protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	EnemyStates currentState;

private:

	void StartAIBehavior();
	
	UPROPERTY(EditAnywhere, Category = "Initialization")
	UBehaviorTree* behaviorTree = nullptr;

	UBlackboardComponent* blackboard = nullptr;
	
	APawnParasite* playerRef = nullptr;
	APawnEnemy* enemyRef = nullptr;
};