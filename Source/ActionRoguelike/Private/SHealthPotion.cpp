// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASHealthPotion::ASHealthPotion() {
	PotionCost = 10;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if(!AttributeComp || AttributeComp->IsMaxHealth())
		return;

	
	// ASPlayerState* PlayerState = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
	// if(PlayerState) {

	if(ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>()) {
		if(PlayerState->GetCredits() < PotionCost)
			return;
		
		PlayerState->UseCredits(PotionCost);
	}
    
    if(AttributeComp->ApplyHealthChange(this, AttributeComp->GetMaxHealth()))
		Super::Interact_Implementation(InstigatorPawn);
}


