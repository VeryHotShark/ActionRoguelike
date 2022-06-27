// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
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
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	float RespawnDelay = 10.0f;

public:	

	void EnableMesh(bool State);
};
