// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "State.generated.h"

class UStateMachine;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPARASITE_API UState : public UObject
{
	GENERATED_BODY()

public:
	friend class UStateMachine;

protected:
	virtual void Init(UStateMachine* stateMachine);
	
	virtual void Start();
	virtual void Update();
	virtual void Exit();

	UStateMachine* stateMachine = nullptr;
		
};
