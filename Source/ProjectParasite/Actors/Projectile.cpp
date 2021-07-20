// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "ProjectParasite/Pawns/PawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/RangedWeapon.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "DestructibleComponent.h"

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

	projectileMesh->AddImpulse(GetActorForwardVector() * moveForce);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Destroy projectile after life time
	if(lifeTimer < lifeTime)
	{
		lifeTimer += DeltaTime;
	}
	else
	{
		Destroy();
	}
}

void AProjectile::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp,
	FVector normalImpulse, const FHitResult& result)
{
	//TODO: Change this to OnOverlap instead to support shooting through glass panels
	
	//Consider bullet hit if hit actor isn't itself, the owning actor (the shooter), or another bullet
	if(otherActor != this && otherActor != GetOwner() && Cast<AProjectile>(otherActor) == nullptr)
	{
		UGameplayStatics::ApplyDamage(otherActor, damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), weaponRef->GetWeaponHolder(), damageType);

		if(!Cast<UDestructibleComponent>(otherComp))
		{
			//Destroy bullet on hit
			Destroy();	
		}
	}
}

