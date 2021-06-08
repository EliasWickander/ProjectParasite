// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"


void UStateMachine::SetOwner(AActor* owningActor)
{
	owner = owningActor;
}

AActor* UStateMachine::GetOwner()
{
	return owner;
}

void UStateMachine::Update()
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
			currentState->OnStateTransition.RemoveDynamic(this, &UStateMachine::SetState);
		}

		currentState = nextState;
		currentStateID = nextStateID;

		currentState->OnStateTransition.AddDynamic(this, &UStateMachine::SetState);
		currentState->Start();
	}
}


void UStateMachine::SetState(FString stateName)
{
	
	if(states.Find(stateName) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("State %s doesn't exist in dictionary"), *stateName);
		return;
	}
	
	if(currentState != states[stateName])
	{
		nextState = states[stateName];
		nextStateID = stateName;
	}
}

void UStateMachine::AddState(FString stateName, UState* state)
{
	//Find a way to make them visible in details dynamically
	states.Append(TMap<FString, UState*> {{stateName, state}});
	state->Init(GetOwner());
	//If this is the first state added, set it as first state to play
	if(states.Num() == 1)
	{
		SetState(stateName);	
	}

	
}


