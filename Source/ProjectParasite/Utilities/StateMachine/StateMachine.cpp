// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"

// Sets default values for this component's properties
UStateMachine::UStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

// Called when the game starts
void UStateMachine::BeginPlay()
{
	Super::BeginPlay();
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

//Needs to be called from the constructor
void UStateMachine::AddState(FString stateName, UState* state)
{

	if(states.FindKey(state))
		return;

	state->SetupAttachment(this);
	//Find a way to make them visible in details dynamically
	states.Append(TMap<FString, UState*> {{stateName, state}});
	state->Init(this);
	
	//If this is the first state added, set it as first state to play
	if(states.Num() == 1)
	{
		SetState(stateName);	
	}

	
}


