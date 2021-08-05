// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Perception/PawnSensingComponent.h"
#include "BTTask_Attack.generated.h"

class APawnEnemy;
class APawnParasite;
class AAIControllerBase;
class APawnBase;

struct BTTaskAttackMemory
{
	UBlackboardComponent* blackboard = nullptr;
	APawnEnemy* ownerEnemy = nullptr;
	AAIControllerBase* enemyAIController = nullptr;
	
	APawnBase* targetActor = nullptr;

	float backOffTimer = 0;
	bool startBackOff = false;

	float attackTimer = 0;
	bool preparingAttack = false;
};

UCLASS()
class PROJECTPARASITE_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Attack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	virtual uint16 GetInstanceMemorySize() const override;
	
	void SetTarget(APawnBase* target, uint8* nodeMemory);

	void OnTargetDeath(AActor* deadActor, uint8* nodeMemory);

	bool IsInRange(uint8* nodeMemory);
	void RotateWeaponToTarget(uint8* nodeMemory);

	bool Retreat(uint8* nodeMemory);

	UBehaviorTreeComponent* behaviorTreeComponent = nullptr;

	APawnBase* playerRef = nullptr;

	void OnTargetDeath(APawnBase* deadPawn, AActor* causerActor);
};
