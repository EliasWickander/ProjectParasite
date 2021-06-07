// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../State.h"
#include "Player_State_Possess.generated.h"

class APawnEnemy;
class APawnParasite;

UCLASS()
class PROJECTPARASITE_API UPlayer_State_Possess : public UState
{
	GENERATED_BODY()

public:
	UPlayer_State_Possess();

	enum PossessState
	{
		PrePossess,
		Possess,
		PostPossess
	};

	PossessState currentState;
	
protected:
	
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:

	void HandlePossessionLoop();
	void MoveToEnemyNape();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float attachLocationLerpSpeed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float attachRotationLerpSpeed = 50;

	APawnParasite* controller = nullptr;
	APawnEnemy* possessedEnemy = nullptr;
};
