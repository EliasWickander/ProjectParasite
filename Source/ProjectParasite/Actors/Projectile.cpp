// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "ProjectParasite/Pawns/PawnBase.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = projectileMesh;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	projectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Move projectile forward
	AddActorLocalOffset(FVector::ForwardVector * moveSpeed, true);

	//Destroy projectile after life time
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
	//Consider bullet hit if hit actor isn't itself, the owning actor (the shooter), or another bullet
	if(otherActor != this && otherActor != GetOwner() && Cast<AProjectile>(otherActor) == nullptr)
	{
		APawnBase* hitPawn = Cast<APawnBase>(otherActor);

		//Apply damage if hit actor is a pawn
		if(hitPawn != nullptr)
		{
			UGameplayStatics::ApplyDamage(hitPawn, damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, damageType);
		}

		//Destroy bullet on hit
		Destroy();	
	}
}

