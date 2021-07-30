// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreHandler.h"

void UScoreHandler::AddScore(ScoreType type, float score)
{
	switch(type)
	{
		case UScoreHandler::Lethality:
		{
			lethalityScore += score;
			break;
		}
		case UScoreHandler::Combo:
		{
			comboScore += score;
			break;
		}
		case UScoreHandler::TimeBonus:
		{
			timeBonusScore += score;
			break;
		}
	}

	totalScore += score;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Totalscore: %i"), totalScore));
}

void UScoreHandler::Copy(UScoreHandler* scoreHandler)
{
	lethalityScore = scoreHandler->lethalityScore;
	comboScore = scoreHandler->comboScore;
	timeBonusScore = scoreHandler->timeBonusScore;
	totalScore = scoreHandler->totalScore;
}
