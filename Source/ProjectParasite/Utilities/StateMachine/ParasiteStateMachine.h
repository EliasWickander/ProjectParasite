// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
#include "ParasiteStateMachine.generated.h"

class UPlayer_State_Idle;
class UPlayer_State_Dash;
class UPlayer_State_Possess;

UCLASS()
class PROJECTPARASITE_API UParasiteStateMachine : public UStateMachine
{
	GENERATED_BODY()

public:
	friend class APawnParasite;
	
	UParasiteStateMachine();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	UPlayer_State_Idle* idleState;

	UPROPERTY()
	UPlayer_State_Dash* dashState;

	UPROPERTY()
	UPlayer_State_Possess* possessState;
};
