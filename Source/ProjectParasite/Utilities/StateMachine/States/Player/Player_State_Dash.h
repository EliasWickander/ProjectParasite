// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../State.h"

#include "Player_State_Dash.generated.h"

class APawnParasite;
/**
 * 
 */
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

	APawnParasite* controller = nullptr;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	bool dashInRotationDir = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float dashTime = 2;

	FVector dashDir = FVector::ZeroVector;

	float timer = 0;
};
