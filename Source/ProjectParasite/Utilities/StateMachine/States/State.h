// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "State.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateTransition, FString, location);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPARASITE_API UState : public UObject
{
	GENERATED_BODY()

public:
	friend class UStateMachine;

	FOnStateTransition OnStateTransition;

protected:
	virtual void Init(AActor* owner);
	
	virtual void Start();
	virtual void Update();
	virtual void Exit();
		
};
