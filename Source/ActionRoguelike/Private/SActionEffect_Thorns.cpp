// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SAttributeComponent.h"

USActionEffect_Thorns::USActionEffect_Thorns() {
	Fraction = 0.2f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Instigator);
	AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator) {
	Super::StopAction_Implementation(Instigator);
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Instigator);
	AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta) {
	if(InstigatorActor == OwningComp->GetOwner())
		return;

	if(USAttributeComponent* InstigatorAttributeComp = USAttributeComponent::GetAttributes(InstigatorActor)) {
		int DamageFraction = FMath::RoundToInt(Delta * Fraction);
		InstigatorAttributeComp->ApplyHealthChange(OwningComp->GetOwner(), DamageFraction);
	} 
}
