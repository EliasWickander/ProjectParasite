// Fill out your copyright notice in the Description page of Project Settings.


#include "State.h"

// Sets default values for this component's properties
UState::UState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UState::Init(UStateMachine* owningStateMachine)
{
	stateMachine = owningStateMachine;
}

void UState::Start()
{
	
}

void UState::Update()
{
	
}

void UState::Exit()
{
	
}




