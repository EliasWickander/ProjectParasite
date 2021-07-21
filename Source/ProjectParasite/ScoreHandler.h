// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScoreHandler.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPARASITE_API UScoreHandler : public UObject
{
	GENERATED_BODY()

public:
	enum ScoreType
	{
		Lethality,
		Combo,
		TimeBonus,
	};

	void AddScore(ScoreType type, float score);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int lethalityScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int comboScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int timeBonusScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int totalScore;
	
};
