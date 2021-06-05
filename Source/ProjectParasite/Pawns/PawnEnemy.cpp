// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemy.h"

#include "ProjectParasite/PlayerControllers/ParasitePlayerController.h"

#include "ProjectParasite/Pawns/PawnParasite.h"

#include "EngineUtils.h"

APawnEnemy::APawnEnemy()
{

}

void APawnEnemy::BeginPlay()
{
	Super::BeginPlay();

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

	if(playersInWorld.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No parasite found in world."))
	}

	playerRef = playersInWorld[0];
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
}

void APawnEnemy::Unpossess()
{	
	playerControllerRef->Possess(playerRef);	
}

void APawnEnemy::Attack()
{
	
}