// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"

#include "P_State_Dash.generated.h"

class APawnParasite;
/**
 * 
 */
UCLASS()
class PROJECTPARASITE_API UP_State_Dash : public UState
{
	GENERATED_BODY()

public:
	UP_State_Dash();
	
protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:

	APawnParasite* controller;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	bool dashInRotationDir = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float dashTime = 2;

	FVector dashDir = FVector::ZeroVector;

	float timer = 0;
};
