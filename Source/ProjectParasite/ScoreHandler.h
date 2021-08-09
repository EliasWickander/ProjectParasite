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

	friend class UGameManager;
public:
	enum ScoreType
	{
		Lethality,
		Combo,
		TimeBonus,
	};

	void AddScore(ScoreType type, float score);
	void Copy(UScoreHandler* scoreHandler);
	int GetTotalScore() { return totalScore; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int lethalityScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int comboScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int timeBonusScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int totalScore = 0;
	
};
