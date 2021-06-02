// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnParasite.h"

#include "DrawDebugHelpers.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "EngineUtils.h"
#include "ProjectParasite/Components/Debug/ParasiteDebugComponent.h"
#include "ProjectParasite/PlayerControllers/ParasitePlayerController.h"

APawnParasite::APawnParasite()
{
	parasiteDebugger = CreateDefaultSubobject<UParasiteDebugComponent>(TEXT("Debug Component"));
}

// Called when the game starts or when spawned
void APawnParasite::BeginPlay()
{

	if(playerControllerRef)
	{
		playerControllerRef->SetPlayerInputEnabled(true);
	}
	
	Super::BeginPlay();
	
}

// Called every frame
void APawnParasite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

// Called to bind functionality to input
void APawnParasite::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Possess", EInputEvent::IE_Pressed, this, &APawnParasite::PossessClosestEnemyInRadius);

}

void APawnParasite::PossessClosestEnemyInRadius()
{
	TArray<APawnEnemy*> enemiesInRadius;

	//Find all enemies in world that are in radius of player
 	for (TActorIterator<APawnEnemy> enemy(GetWorld()); enemy; ++enemy)
	{
		APawnEnemy* enemyFound = *enemy;

 		if(FVector::Dist(enemy->GetActorLocation(), GetActorLocation()) <= possessRadius)
 			enemiesInRadius.Add(enemyFound);
	}

	//If there are enemies in radius, possess the first one
	if(enemiesInRadius.Num() <= 0)
		return;

	playerControllerRef->Possess(enemiesInRadius[0]);
	
	for(APawnEnemy* e : enemiesInRadius)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *e->GetName());
	}
}