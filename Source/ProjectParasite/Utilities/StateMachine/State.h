// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTPARASITE_API State
{
public:
	State(AActor* caller);
	~State();

	float exitTime = 0;

	virtual void Start();
	virtual void Update();
	virtual void Exit();

	AActor* caller;
protected:
};
