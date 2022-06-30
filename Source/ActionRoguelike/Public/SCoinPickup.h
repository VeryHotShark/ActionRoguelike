// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPotion.h"
#include "GameFramework/Actor.h"
#include "SCoinPickup.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCoinPickup : public ASPotion
{
	GENERATED_BODY()
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
public:	
	// Sets default values for this actor's properties
	ASCoinPickup();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Coin")
	int CreditsAmount = 10;

};
