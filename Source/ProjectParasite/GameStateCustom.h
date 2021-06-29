// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateCustom.generated.h"

UCLASS()
class PROJECTPARASITE_API AGameStateCustom : public AGameStateBase
{
	GENERATED_BODY()

public:
	void OpenNextLevel();
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	FName levelPrefix = "Level";

	TMap<FString, int> levelMap;
};
