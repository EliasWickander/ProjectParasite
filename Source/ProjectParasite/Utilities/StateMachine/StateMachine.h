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

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetState(FString stateName);
	void AddState(FString stateName, UState* state);

	UState* currentState = nullptr;
	UState* nextState = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	TMap<FString, UState*> states;
		
};
