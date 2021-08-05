// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LookAround.generated.h"

class APawnEnemy;
struct BTTaskLookAroundMemory
{
	APawnEnemy* ownerEnemy = nullptr;

	float stateTimer = 0;

	float lookAroundTimer = 0;
	FRotator startRot = FRotator::ZeroRotator;
	FRotator currentRot = FRotator::ZeroRotator;
	FRotator targetRot = FRotator::ZeroRotator;
	float targetAngle = 0;
};

UCLASS()
class PROJECTPARASITE_API UBTTask_LookAround : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_LookAround();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	void Search(uint8* NodeMemory);

	bool LookAtRotation(float duration, uint8* NodeMemory);

private:
	virtual uint16 GetInstanceMemorySize() const override;
};
