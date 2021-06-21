// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Detect.generated.h"

struct BTTaskDetectMemory
{
	class APawnEnemy* ownerEnemy = nullptr;
	UBlackboardComponent* blackboard = nullptr;
};

UCLASS()
class PROJECTPARASITE_API UBTTask_Detect : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Detect();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	void Detect(uint8* nodeMemory);
	
	virtual uint16 GetInstanceMemorySize() const override;

	class APawnParasite* playerRef = nullptr;

	bool hasDetected = false;

	float reactionTimer = 0;
};
