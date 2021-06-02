// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "State.h"

/**
 * 
 */
class PROJECTPARASITE_API StateMachine
{
public:
	StateMachine(FString rootStateName, State* rootState);
	~StateMachine();
	void Update();
	void SetState(FString stateName);

	State* currentState = nullptr;
	State* nextState = nullptr;

private:

	TMap<FString, State*> states;
};
