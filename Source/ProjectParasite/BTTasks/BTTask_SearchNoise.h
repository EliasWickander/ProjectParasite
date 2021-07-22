// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SearchNoise.generated.h"

class APawnEnemy;
struct BTTaskSearchNoiseMemory
{
	APawnEnemy* ownerEnemy = nullptr;
	bool reachedGoal = false;
};

UCLASS()
class PROJECTPARASITE_API UBTTask_SearchNoise : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SearchNoise();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;


private:
	virtual uint16 GetInstanceMemorySize() const override;

	float reactionTimer = 0;

	float lookAroundTimer = 0;

};
