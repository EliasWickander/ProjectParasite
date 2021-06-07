// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemy.h"

#include "ProjectParasite/PlayerControllers/PlayerControllerParasite.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "AIController.h"
#include "EngineUtils.h"

APawnEnemy::APawnEnemy()
{
	napeComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Nape"));
	napeComponent->SetupAttachment(baseMesh);
}

void APawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	playerRef = FindPlayerInWorld();

	AIController = Cast<AAIController>(GetController());

	AIController->SetFocus(playerRef);
}

void APawnEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Possess", IE_Pressed, this, &APawnEnemy::Unpossess);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnEnemy::Attack);
}


void APawnEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(IsPlayerControlled())
		RotateToMouseCursor();

}

void APawnEnemy::Unpossess()
{
	playerControllerRef->Possess(playerRef);
	SetPossessed(false);
}

void APawnEnemy::SetPossessed(bool isPossessed)
{
	if(isPossessed)
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	else
	{
		AIController->SetFocus(playerRef);
		AIController->Possess(this);	
	}
}

void APawnEnemy::Attack()
{
	
}

APawnParasite* APawnEnemy::FindPlayerInWorld()
{
	//Find all parasites in world (should only be one)
	TArray<APawnParasite*> playersInWorld;
	
	for (TActorIterator<APawnParasite> p(GetWorld()); p; ++p)
	{
		APawnParasite* parasiteFound = *p;
		playersInWorld.Add(parasiteFound);
	}

	if(playersInWorld.Num() > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("More than one parasite found in world."))
	}

	//Crash game if no parasite is in world since that would break enemy behaviour
	if(playersInWorld.Num() <= 0)
	{
		UE_LOG(LogTemp, Fatal, TEXT("No parasite found in world."))
	}

	return playersInWorld[0];
}