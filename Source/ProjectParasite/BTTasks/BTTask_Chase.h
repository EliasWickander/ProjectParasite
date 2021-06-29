// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Chase.generated.h"

class APawnBase;
class APawnEnemy;
class APawnParasite;
class AAIControllerBase;

struct BTTaskChaseMemory
{
	APawnEnemy* ownerEnemy = nullptr;
	UBlackboardComponent* blackboard = nullptr;
	AAIControllerBase* enemyAIController = nullptr;
	APawnBase* targetActor = nullptr;
};

UCLASS()
class PROJECTPARASITE_API UBTTask_Chase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Chase();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	virtual uint16 GetInstanceMemorySize() const override;
	
	void SetTarget(APawnBase* target, BTTaskChaseMemory* instanceMemory);

	UBehaviorTreeComponent* behaviorTreeComponent = nullptr;
	APawnParasite* playerRef = nullptr;
};
