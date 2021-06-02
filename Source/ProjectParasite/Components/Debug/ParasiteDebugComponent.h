// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParasiteDebugComponent.generated.h"

class APawnParasite;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPARASITE_API UParasiteDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UParasiteDebugComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	APawnParasite* parasiteRef = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "General")
	bool debugEnabled = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Possession")
	int32 pSphereSegments = 50;

	UPROPERTY(EditDefaultsOnly, Category = "Possession")
	FColor pSphereColor = FColor::Blue;
	
	void DrawPossessionRadius();
		
};
