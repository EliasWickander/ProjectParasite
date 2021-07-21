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
}
