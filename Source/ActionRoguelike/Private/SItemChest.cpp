// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	SetReplicates(true);
}

void ASItemChest::OnActorLoaded_Implementation() {
	OnRep_LidOpened();
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn) {
	bLidOpen = !bLidOpen;
	OnRep_LidOpened();
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpen);
}

void ASItemChest::OnRep_LidOpened() {
	float CurrPitch = bLidOpen ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}
