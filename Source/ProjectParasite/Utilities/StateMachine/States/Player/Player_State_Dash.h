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
	
protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	
	APawnParasite* playerRef = nullptr;

	FVector dashDir = FVector::ZeroVector;
	
	float dashTimer = 0;
	float prevMoveSpeed;
};
