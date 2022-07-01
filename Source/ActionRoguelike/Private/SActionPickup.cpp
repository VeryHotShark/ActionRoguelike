// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionPickup.h"

#include "SActionComponent.h"

void ASActionPickup::Interact_Implementation(APawn* InstigatorPawn) {
	USActionComponent* ActionComp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));

	if(!ActionComp || ActionComp->HasAction(ActionToGrant))
		return;

	ActionComp->AddAction(InstigatorPawn,ActionToGrant);
	Super::Interact_Implementation(InstigatorPawn);
}
