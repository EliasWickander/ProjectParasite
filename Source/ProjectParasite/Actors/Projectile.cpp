// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "ProjectParasite/Pawns/PawnBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = projectileMesh;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	projectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(FVector::ForwardVector * moveSpeed, true);
	
	if(timer < lifeTime)
	{
		timer += DeltaTime;
	}
	else
	{
		Destroy();
	}
}

void AProjectile::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp,
	FVector normalImpulse, const FHitResult& result)
{
	if(otherActor != this && otherActor != GetOwner() && Cast<AProjectile>(otherActor) == nullptr)
	{
		APawnBase* hitPawn = Cast<APawnBase>(otherActor);

		if(hitPawn != nullptr)
		{
			UGameplayStatics::ApplyDamage(hitPawn, damage, hitPawn->Controller, this, damageType);
		}

		Destroy();	
	}
}

