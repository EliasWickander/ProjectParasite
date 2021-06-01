// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraFollower.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACameraFollower::ACameraFollower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;
	
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);
	
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);

}

// Called when the game starts or when spawned
void ACameraFollower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

