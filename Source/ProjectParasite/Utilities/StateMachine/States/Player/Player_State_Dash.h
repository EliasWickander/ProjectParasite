// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../State.h"

#include "Player_State_Dash.generated.h"

class APawnParasite;

UCLASS()
class PROJECTPARASITE_API UPlayer_State_Dash : public UState
{
	GENERATED_BODY()

public:
	UPlayer_State_Dash();
	
protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float dashTime = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float dashSpeed = 500;
	
	APawnParasite* controller = nullptr;

	FVector dashDir = FVector::ZeroVector;
	
	float timer = 0;
	float prevMoveSpeed;
};
