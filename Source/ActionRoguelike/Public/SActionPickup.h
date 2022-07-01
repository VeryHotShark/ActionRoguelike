// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SPotion.h"
#include "SActionPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASActionPickup : public ASPotion
{
	GENERATED_BODY()

protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:

	UPROPERTY(EditDefaultsOnly, Category="Action")
	TSubclassOf<USAction> ActionToGrant;
	
};
