// Fill out your copyright notice in the Description page of Project Settings.


#include "ParasiteStateMachine.h"

#include "States/Player/Player_State_Idle.h"
#include "States/Player/Player_State_Dash.h"
#include "States/Player/Player_State_Possess.h"

UParasiteStateMachine::UParasiteStateMachine()
{
	//Make generic method for this somehow?
	if (idleState == nullptr)
	{
		idleState = CreateDefaultSubobject<UPlayer_State_Idle>(TEXT("State_Idle"));
		AddState(TEXT("State_Idle"), idleState);
	}
	
	if(dashState == nullptr)
	{
		dashState = CreateDefaultSubobject<UPlayer_State_Dash>(TEXT("State_Dash"));
		AddState(TEXT("State_Dash"), dashState);	
	}

	if(possessState == nullptr)
	{
		possessState = CreateDefaultSubobject<UPlayer_State_Possess>(TEXT("State_Possess"));
		AddState(TEXT("State_Possess"), possessState);	
	}
}