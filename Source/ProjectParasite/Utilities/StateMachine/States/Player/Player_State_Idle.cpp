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

	if(playerRef->GetActorLocation().Z != playerRef->GetPlayerStartPos().Z)
	{
		FVector targetPos = playerRef->GetActorLocation();
		targetPos.Z = playerRef->GetPlayerStartPos().Z;

		if(FVector::Dist(playerRef->GetActorLocation(), targetPos) > 0.05f)
		{
			FVector currentLocation = FMath::Lerp(playerRef->GetActorLocation(), targetPos, 5 * GetWorld()->GetDeltaSeconds());

			playerRef->SetActorLocation(currentLocation, true);
		}
	}
}

void UPlayer_State_Idle::Exit()
{
}
