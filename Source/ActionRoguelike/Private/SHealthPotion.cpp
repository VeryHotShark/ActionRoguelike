// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

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



FText ASHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn) {
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);

	if(AttributeComponent && AttributeComponent->IsMaxHealth())
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health");
	
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum"), PotionCost);
}

#undef LOCTEXT_NAMESPACE