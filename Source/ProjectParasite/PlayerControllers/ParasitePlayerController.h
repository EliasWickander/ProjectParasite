// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ParasitePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPARASITE_API AParasitePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AParasitePlayerController();

	void SetPlayerInputEnabled(bool enabled);
	
private:
};
