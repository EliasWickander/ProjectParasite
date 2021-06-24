// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class APawnParasite;
class APawnShooter;

UENUM(BlueprintType)
enum class ShooterStates : uint8
{
	State_Idle UMETA(DisplayName = "Idle"),
	State_Patrol UMETA(DisplayName = "Patrol"),
	State_Chase UMETA(DisplayName = "Chase"),
	State_Attack UMETA(DisplayName = "Attack"),
	State_Stunned UMETA(DisplayName = "Stunned")
};

UCLASS()
class PROJECTPARASITE_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	void SetCurrentState(ShooterStates state);
	ShooterStates GetCurrentState() { return currentState; }

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ShooterStates currentState;

private:

	void StartAIBehavior();
	
	UPROPERTY(EditAnywhere)
	UBehaviorTree* behaviorTree = nullptr;

	UBlackboardComponent* blackboard = nullptr;
	
	APawnParasite* playerRef = nullptr;
	APawnShooter* shooterRef = nullptr;

};
