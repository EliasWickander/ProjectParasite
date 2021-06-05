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

	timer = 0;
	
	if(dashInRotationDir)
	{
		dashDir = controller->GetActorForwardVector();
	}
	else
	{
		//Dash in direction of input
	}

	dashDir.Normalize();

	controller->SetCanMove(false);
}

void UPlayer_State_Dash::Update()
{
	if(timer < dashTime)
	{
		timer += GetWorld()->DeltaTimeSeconds;
		
		controller->AddMovementInput(dashDir);	
	}
	else
	{
		OnStateTransition.Broadcast("State_Idle");
	}
}

void UPlayer_State_Dash::Exit()
{
	controller->SetCanMove(true);
}
