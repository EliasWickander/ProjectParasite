// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Guard.generated.h"

class APawnEnemy;

struct BTTaskGuardMemory
{
	TQueue<FVector>* guardPointQueue;
	FVector currentGuardPoint = FVector::ZeroVector;
	APawnEnemy* ownerEnemy = nullptr;
};

UCLASS()
class PROJECTPARASITE_API UBTTask_Guard : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Guard();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	void SetupGuardPoints(BTTaskGuardMemory* instanceMemory);
	void InitNextGuardPoint(BTTaskGuardMemory* instanceMemory);
	
	virtual uint16 GetInstanceMemorySize() const override;

	float guardTimer = 0;
};
