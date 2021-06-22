// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

void AKnife::Tick(float DeltaSeconds)
{

}

void AKnife::Trigger()
{
	Super::Trigger();

	TArray<AActor*> overlappingActors;
	
	GetOverlappingActors(overlappingActors, APawnEnemy::StaticClass());

	for(AActor* actor : overlappingActors)
	{
		APawnEnemy* hitEnemy = Cast<APawnEnemy>(actor);

		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		
		if(hitEnemy->GetController() != playerController)
		{
			UGameplayStatics::ApplyDamage(hitEnemy, damage, playerController, this, damageType);
			return;
		}
	}
}
