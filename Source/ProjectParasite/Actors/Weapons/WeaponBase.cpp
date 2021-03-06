// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/SphereComponent.h"
#include "ProjectParasite/Components/Debug/WeaponDebugComponent.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pickupTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Trigger"));
	RootComponent = pickupTrigger;
	
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Base Mesh"));
	baseMesh->SetupAttachment(pickupTrigger);

	debugComponent = CreateDefaultSubobject<UWeaponDebugComponent>("Debug Component");
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::Use()
{

}

