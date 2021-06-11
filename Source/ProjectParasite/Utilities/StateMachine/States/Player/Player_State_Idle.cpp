// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_State_Idle.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Utilities/StateMachine/StateMachine.h"

void UPlayer_State_Idle::Start()
{
	controller = Cast<APawnParasite>(stateMachine->GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("Start Idle"));
	
}

void UPlayer_State_Idle::Update()
{
	controller->RotateToMouseCursor();
}

void UPlayer_State_Idle::Exit()
{
}
