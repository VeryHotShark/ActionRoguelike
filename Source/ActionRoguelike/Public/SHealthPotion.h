// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPotion.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPotion
{
	GENERATED_BODY()
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	UPROPERTY(EditAnywhere)
	float Delta;
};
