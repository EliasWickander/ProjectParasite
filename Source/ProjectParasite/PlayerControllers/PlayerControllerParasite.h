// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerParasite.generated.h"

UCLASS()
class PROJECTPARASITE_API APlayerControllerParasite : public APlayerController
{
	GENERATED_BODY()

public:
	void SetPlayerInputEnabled(bool enabled);
	
protected:
	virtual void BeginPlay() override;
};
