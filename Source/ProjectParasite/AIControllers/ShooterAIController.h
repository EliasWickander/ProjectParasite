// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class ATargetPoint;
class APawnParasite;
class APawnShooter;

UCLASS()
class PROJECTPARASITE_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:

	void StartAIBehavior();
	UPROPERTY(EditAnywhere)
	UBehaviorTree* behaviorTree = nullptr;

	UBlackboardComponent* blackboard = nullptr;
	
	APawnParasite* playerRef = nullptr;
	APawnShooter* shooterRef = nullptr;

};
