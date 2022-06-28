// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent() {
	Health = MaxHealth;
}

void USAttributeComponent::BeginPlay() {
	Super::BeginPlay();
	Health = MaxHealth;
}

bool USAttributeComponent::IsAlive() const {
	return Health > 0.0f;
}

bool USAttributeComponent::IsMaxHealth() const {
	return Health >= MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(float Delta) {
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return ActualDelta != 0;
}

float USAttributeComponent::GetHealth() const {
	return Health;
}

float USAttributeComponent::GetMaxHealth() const{
	return  MaxHealth;
}

