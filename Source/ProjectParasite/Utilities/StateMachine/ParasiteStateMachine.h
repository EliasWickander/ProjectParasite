// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
#include "ParasiteStateMachine.generated.h"

class UP_State_Idle;
class UP_State_Dash;

UCLASS()
class PROJECTPARASITE_API UParasiteStateMachine : public UStateMachine
{
	GENERATED_BODY()

public:
	UParasiteStateMachine();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "States", meta = (AllowPrivateAccess = "true"))
	UP_State_Idle* idleState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "States", meta = (AllowPrivateAccess = "true"))
	UP_State_Dash* dashState;
};
