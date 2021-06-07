// Fill out your copyright notice in the Description page of Project Settings.


#include "ParasiteStateMachine.h"

#include "States/Player/Player_State_Idle.h"
#include "States/Player/Player_State_Dash.h"
#include "States/Player/Player_State_Possess.h"

UParasiteStateMachine::UParasiteStateMachine()
{
	idleState = CreateDefaultSubobject<UPlayer_State_Idle>(TEXT("State_Idle"));
	idleState->SetupAttachment(this);
	
	dashState = CreateDefaultSubobject<UPlayer_State_Dash>(TEXT("State_Dash"));
	dashState->SetupAttachment(this);
	
	possessState = CreateDefaultSubobject<UPlayer_State_Possess>(TEXT("State_Possess"));
	possessState->SetupAttachment(this);
}

void UParasiteStateMachine::BeginPlay()
{
	Super::BeginPlay();

	AddState(TEXT("State_Idle"), idleState);
	AddState(TEXT("State_Dash"), dashState);
	AddState(TEXT("State_Possess"), possessState);
}
