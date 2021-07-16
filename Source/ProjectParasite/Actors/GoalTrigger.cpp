// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalTrigger.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/GameStateCustom.h"
#include "ProjectParasite/GameModes/EliminationGamemode.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"

// Sets default values
AGoalTrigger::AGoalTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	triggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	RootComponent = triggerVolume;
}

// Called when the game starts or when spawned
void AGoalTrigger::BeginPlay()
{
	Super::BeginPlay();

	triggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AGoalTrigger::OnBeginOverlap);
}

// Called every frame
void AGoalTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoalTrigger::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	AActor* actorToConsider = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if(otherActor == actorToConsider)
	{
		UE_LOG(LogTemp, Warning, TEXT("Goal Triggered"));
		onGoalTriggered.Broadcast();
	}
}