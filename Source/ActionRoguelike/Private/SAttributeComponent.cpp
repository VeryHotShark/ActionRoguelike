// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}

bool USAttributeComponent::IsAlive() const {
	return Health > 0.0f;
}

bool USAttributeComponent::IsMaxHealth() const {
	return Health >= MaxHealth;
}


bool USAttributeComponent::ApplyHealthChange(float Delta) {
	Health += Delta;
	FMath::Clamp(Health, 0, MaxHealth);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

