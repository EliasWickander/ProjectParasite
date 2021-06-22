// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class ATargetPoint;
class APawnParasite;
class APawnShooter;
class UBTTaskNode;
class UBTCompositeNode;

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

	ShooterStates GetStatesEnum() { return states; }
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ShooterStates states;

private:

	void StartAIBehavior();
	
	
	
	UPROPERTY(EditAnywhere)
	UBehaviorTree* behaviorTree = nullptr;

	UBlackboardComponent* blackboard = nullptr;
	
	APawnParasite* playerRef = nullptr;
	APawnShooter* shooterRef = nullptr;

};
