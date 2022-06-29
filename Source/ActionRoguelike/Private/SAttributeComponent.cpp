// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor) {
	if(FromActor) 
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(StaticClass()));

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor) {
	USAttributeComponent* AttributeComp = GetAttributes(Actor);

	if(AttributeComp)
		return AttributeComp->IsAlive();

	return false;
}

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent() {
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

void USAttributeComponent::BeginPlay() {
	Super::BeginPlay();
	Health = MaxHealth;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor) {
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

bool USAttributeComponent::IsAlive() const {
	return Health > 0.0f;
}

bool USAttributeComponent::IsMaxHealth() const {
	return Health >= MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta) {
	if(!GetOwner()->CanBeDamaged())
		return  false;
	
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	
	return ActualDelta != 0;
}

float USAttributeComponent::GetHealth() const {
	return Health;
}

float USAttributeComponent::GetMaxHealth() const{
	return  MaxHealth;
}

