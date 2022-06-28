// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SPotion.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASPotion : public AActor,public ISGameplayInterface
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
public:	
	// Sets default values for this actor's properties
	ASPotion();

protected:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere)
	float RespawnDelay = 10.0f;

	FTimerHandle TimerHandle_RespawnDelay;
public:	

	void HideAndCooldownPowerup();
	void SetPowerUpState(bool State);
};
