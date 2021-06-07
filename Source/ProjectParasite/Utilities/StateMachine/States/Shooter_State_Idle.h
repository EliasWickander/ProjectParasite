// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "Shooter_State_Idle.generated.h"

UCLASS()
class PROJECTPARASITE_API UShooter_State_Idle : public UState
{
	GENERATED_BODY()

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;
	
};
