// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_State_Idle.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Utilities/StateMachine/StateMachine.h"

void UPlayer_State_Idle::Start()
{
	playerRef = Cast<APawnParasite>(stateMachine->GetOwner());
	
}

void UPlayer_State_Idle::Update()
{
	playerRef->RotateToMouseCursor();
}

void UPlayer_State_Idle::Exit()
{
}
