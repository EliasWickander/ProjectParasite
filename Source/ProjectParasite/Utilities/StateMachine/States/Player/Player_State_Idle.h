// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../State.h"
#include "Player_State_Idle.generated.h"

class APawnParasite;
UCLASS()
class PROJECTPARASITE_API UPlayer_State_Idle : public UState
{
	GENERATED_BODY()

public:
	UPlayer_State_Idle();
	
protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	APawnParasite* controller = nullptr;
};
