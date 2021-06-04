// Fill out your copyright notice in the Description page of Project Settings.


#include "ParasiteStateMachine.h"

#include "States/P_State_Idle.h"
#include "States/P_State_Dash.h"

UParasiteStateMachine::UParasiteStateMachine()
{
	if (idleState == nullptr)
	{
		idleState = CreateDefaultSubobject<UP_State_Idle>(TEXT("State_Idle"));
		AddState(TEXT("State_Idle"), idleState);
	}
	
	if(dashState == nullptr)
	{
		dashState = CreateDefaultSubobject<UP_State_Dash>(TEXT("State_Dash"));
		AddState(TEXT("State_Dash"), dashState);	
	}
}