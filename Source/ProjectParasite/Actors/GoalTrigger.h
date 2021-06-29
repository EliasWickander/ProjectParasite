// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalTrigger.generated.h"

class AGameStateCustom;
class AEliminationGamemode;
class APawnParasite;
class UBoxComponent;

UCLASS()
class PROJECTPARASITE_API AGoalTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalTrigger();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* triggerVolume = nullptr;

	AEliminationGamemode* gameModeRef = nullptr;
	AGameStateCustom* gameStateRef = nullptr;
	
	APawnParasite* playerRef = nullptr;
};
