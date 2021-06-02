// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../State.h"

/**
 * 
 */
class PROJECTPARASITE_API SState_Idle : public State
{
public:
	SState_Idle(AActor* caller);

	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;
};
