// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_State_Dash.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

UPlayer_State_Dash::UPlayer_State_Dash()
{
	controller = Cast<APawnParasite>(GetOwner());
}

void UPlayer_State_Dash::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Dash"));

	UE_LOG(LogTemp, Warning, TEXT("%f"), controller->GetMoveSpeed());
	UE_LOG(LogTemp, Warning, TEXT("%f"), dashTime);
	
	timer = 0;
	
	dashDir = controller->GetActorForwardVector();
	dashDir.Normalize();

	prevMoveSpeed = controller->GetMoveSpeed();
	
	controller->SetMoveSpeed(dashSpeed);

	//Prevent player from moving during dash
	controller->SetCanMove(false);
}

void UPlayer_State_Dash::Update()
{
	if(timer < dashTime)
	{
		timer += GetWorld()->DeltaTimeSeconds;
		
		controller->AddMovementInput(dashDir);	
		
		controller->PossessClosestEnemyInRadius();
	}
	else
	{
		OnStateTransition.Broadcast("State_Idle");
	}
}

void UPlayer_State_Dash::Exit()
{
	controller->SetMoveSpeed(prevMoveSpeed);
	controller->SetCanMove(true);
}
