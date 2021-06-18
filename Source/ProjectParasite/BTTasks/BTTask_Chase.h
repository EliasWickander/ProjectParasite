// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Chase.generated.h"

UCLASS()
class PROJECTPARASITE_API UBTTask_Chase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Chase();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	UPROPERTY(EditAnywhere)
	float acceptanceRadius = 200;

private:
	virtual uint16 GetInstanceMemorySize() const override;

};