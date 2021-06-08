// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_State_Idle.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

void UPlayer_State_Idle::Init(AActor* owner)
{
	controller = Cast<APawnParasite>(owner);
}

void UPlayer_State_Idle::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Idle"));
	
}

void UPlayer_State_Idle::Update()
{
	controller->RotateToMouseCursor();
}

void UPlayer_State_Idle::Exit()
{
}
