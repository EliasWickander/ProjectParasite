// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "States/State.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPARASITE_API UStateMachine : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStateMachine();

	void Update();
	
	UFUNCTION()
	void SetState(FString stateName);
	void AddState(FString stateName, UState* state);

	UState* currentState = nullptr;
	FString currentStateID = "";
	
	UState* nextState = nullptr;
	FString nextStateID = "";

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	TMap<FString, UState*> states;
		
};
