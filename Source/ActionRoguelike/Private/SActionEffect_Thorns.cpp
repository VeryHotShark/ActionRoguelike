// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns() {
	Fraction = 0.2f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);
	// USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Instigator);
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator) {
	Super::StopAction_Implementation(Instigator);
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta) {
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	
	if(OwningActor == InstigatorActor)
		return;

	int DamageFraction = FMath::RoundToInt(Delta * Fraction);
	USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, DamageFraction);

	/*
	if(USAttributeComponent* InstigatorAttributeComp = USAttributeComponent::GetAttributes(InstigatorActor)) {
		int DamageFraction = FMath::RoundToInt(Delta * Fraction);
		InstigatorAttributeComp->ApplyHealthChange(OwningComp->GetOwner(), DamageFraction);
	}
	*/
}
