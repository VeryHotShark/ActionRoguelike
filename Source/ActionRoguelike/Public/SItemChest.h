// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public  ISGameplayInterface
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly)
    bool bLidOpen;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch = 110;

};
