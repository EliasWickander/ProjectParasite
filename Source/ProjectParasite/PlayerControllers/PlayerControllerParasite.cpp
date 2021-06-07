// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerParasite.h"

void APlayerControllerParasite::BeginPlay()
{
	SetShowMouseCursor(true);
	EnableInput(this);
}

void APlayerControllerParasite::SetPlayerInputEnabled(bool enabled)
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
