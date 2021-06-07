// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_State_Possess.h"

#include "Components/CapsuleComponent.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/PlayerControllers/PlayerControllerParasite.h"
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

	currentState = PossessState::PrePossess;
	
	controller->GetCollider()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	controller->SetCanMove(false);
	possessedEnemy->SetPossessed(true);
}

void UPlayer_State_Possess::Update()
{
	FVector napeLocation = possessedEnemy->GetNapeComponent()->GetComponentLocation();
	
	if(currentState == PossessState::PrePossess)
	{

		if(FVector::Dist(controller->GetActorLocation(), napeLocation) > 1)
		{
			//TODO: Use linear interpolation instead of exponential
			FVector lerpedPos = FMath::Lerp(controller->GetActorLocation(), napeLocation, attachLocationLerpSpeed * GetWorld()->DeltaTimeSeconds);

			FRotator lerpedRot = FMath::Lerp(controller->GetActorRotation(), possessedEnemy->GetActorForwardVector().Rotation(), attachRotationLerpSpeed * GetWorld()->DeltaTimeSeconds);
			
			controller->SetActorLocation(lerpedPos);
			controller->SetActorRotation(lerpedRot);

			controller->AttachToComponent(possessedEnemy->GetNapeComponent(), FAttachmentTransformRules::KeepWorldTransform);
		}
		else
		{
			controller->playerControllerRef->Possess(possessedEnemy);
			currentState = PossessState::Possess;
		}
	}
	else if(currentState == PossessState::Possess)
	{
		// controller->SetActorLocation(napeLocation);
		//
		// controller->SetActorRotation(possessedEnemy->GetActorForwardVector().Rotation());
		
		if(controller->IsPlayerControlled())
		{
			currentState = PossessState::PostPossess;
		}	
	}
	else if(currentState == PossessState::PostPossess)
	{
		OnStateTransition.Broadcast("State_Idle");	
	}
}

void UPlayer_State_Possess::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit Possess"));

	controller->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	controller->SetActorLocation(possessedEnemy->GetActorLocation() + -possessedEnemy->GetActorForwardVector() * 200);

	controller->GetCollider()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	controller->SetCanMove(true);
}
