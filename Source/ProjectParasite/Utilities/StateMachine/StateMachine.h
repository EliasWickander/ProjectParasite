// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "States/State.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPARASITE_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:
	void SetOwner(AActor* owningActor);
	AActor* GetOwner();
	
	void Update();
	
	UFUNCTION()
	void SetState(FString stateName);

	UFUNCTION()
	void AddState(FString stateName, UState* state);

	UState* currentState = nullptr;
	FString currentStateID = "";
	
	UState* nextState = nullptr;
	FString nextStateID = "";
private:

	TMap<FString, UState*> states;

	AActor* owner = nullptr;
		
};
