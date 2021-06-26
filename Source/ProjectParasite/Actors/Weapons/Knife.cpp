// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"

void AKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKnife::Use()
{
	Super::Use();

	//Get all actors overlapping the knife
	TArray<AActor*> overlappingActors;
	
	GetOverlappingActors(overlappingActors, APawnEnemy::StaticClass());

	for(AActor* actor : overlappingActors)
	{
		//Find the first enemy overlapped and apply damage to it
		APawnEnemy* hitEnemy = Cast<APawnEnemy>(actor);

		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		
		if(hitEnemy->GetController() != playerController)
		{
			UGameplayStatics::ApplyDamage(hitEnemy, damage, playerController, this, damageType);
			return;
		}
	}
}
