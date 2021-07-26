// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatrolBetweenWaypoints.generated.h"

class APawnEnemy;

struct BTTaskPatrolBetweenWaypointsMemory
{
	TQueue<FVector>* patrolPointQueue;
	FVector currentWaypoint = FVector::ZeroVector;
	APawnEnemy* ownerEnemy = nullptr;
	bool reachedGoalLastFrame = false;
};

UCLASS()
class PROJECTPARASITE_API UBTTask_PatrolBetweenWaypoints : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PatrolBetweenWaypoints();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;


private:
	void SetupPatrolPoints(BTTaskPatrolBetweenWaypointsMemory* instanceMemory);
	void InitNextWaypoint(BTTaskPatrolBetweenWaypointsMemory* instanceMemory);
	
	virtual uint16 GetInstanceMemorySize() const override;
};
