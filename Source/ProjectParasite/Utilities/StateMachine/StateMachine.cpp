// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"

StateMachine::StateMachine(FString rootStateName, State* rootState)
{
	states.Append(TMap<FString, State*> {{rootStateName, rootState}});

	SetState(rootStateName);
}

StateMachine::~StateMachine()
{
}

void StateMachine::Update()
{
	if(currentState != nullptr)
	{
		currentState->Update();
	}
	
	if(nextState != currentState)
	{
		if(currentState != nullptr)
		{
			currentState->Exit();
			//unsubscribe from event
		}

		currentState = nextState;

		//subscribe to event
		currentState->Start();
	}
}

void StateMachine::SetState(FString stateName)
{
	if(states.Find(stateName) == nullptr)
		UE_LOG(LogTemp, Error, TEXT("State %s doesn't exist in dictionary"), *stateName);

	nextState = states[stateName];
}

