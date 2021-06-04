// Fill out your copyright notice in the Description page of Project Settings.


#include "ParasitePlayerController.h"

AParasitePlayerController::AParasitePlayerController()
{
	SetShowMouseCursor(true);
}

void AParasitePlayerController::SetPlayerInputEnabled(bool enabled)
{
	if(enabled)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
	}
}
