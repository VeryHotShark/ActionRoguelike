// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if(!AttributeComp || AttributeComp->IsMaxHealth())
		return;
    
    AttributeComp->ApplyHealthChange(Delta);
	EnableMesh(false);

	FTimerHandle TimerHandle_RespawnDelay;
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject( this, &ASPotion::EnableMesh, true);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, RespawnDelegate, RespawnDelay, false);
	
}


