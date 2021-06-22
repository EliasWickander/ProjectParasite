// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Base Mesh"));
	RootComponent = baseMesh;

	pickupTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Trigger"));
	pickupTrigger->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if(isPickedUp)
	{
		pickupTrigger->UnregisterComponent();
	}
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Trigger()
{

}

