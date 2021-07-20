// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "ProjectParasite/Pawns/PawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/RangedWeapon.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "DestructibleComponent.h"
#include "Components/CapsuleComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = projectileMesh;

	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	capsuleComponent->SetupAttachment(RootComponent);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	capsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);

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

void AProjectile::OnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	//TODO: Change this to OnOverlap instead to support shooting through glass panels
	
	//Consider bullet hit if hit actor isn't itself, the owning actor (the shooter), or another bullet
	if(otherActor != this && otherActor != GetOwner() && Cast<AProjectile>(otherActor) == nullptr)
	{
		UGameplayStatics::ApplyDamage(otherActor, damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), weaponRef->GetWeaponHolder(), damageType);

		UDestructibleComponent* destructibleComponent = Cast<UDestructibleComponent>(otherComp);
		
		if(!destructibleComponent)
		{
			//Destroy bullet on hit
			Destroy();	
		}
		else
		{
			FVector impulseDir = destructibleComponent->GetComponentLocation() - GetActorLocation();
			impulseDir.Z = 0;
			impulseDir.Normalize();
			
			destructibleComponent->ApplyDamage(damage, destructibleComponent->GetComponentLocation(), impulseDir, 10);
		}
	}
}

