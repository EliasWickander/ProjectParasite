// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_State_Possess.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/PlayerControllers/ParasitePlayerController.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"

UPlayer_State_Possess::UPlayer_State_Possess()
{
	controller = Cast<APawnParasite>(GetOwner());
}

void UPlayer_State_Possess::Init(APawnEnemy* enemyToPossess)
{
	possessedEnemy = enemyToPossess;
}

void UPlayer_State_Possess::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Possess"));
	controller->playerControllerRef->Possess(possessedEnemy);
}

void UPlayer_State_Possess::Update()
{
}

void UPlayer_State_Possess::Exit()
{
}
