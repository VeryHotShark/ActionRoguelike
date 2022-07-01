// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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
	MaxRage = 100.0f;
	Rage = 0.0f;
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
	if(!GetOwner()->CanBeDamaged() && Delta < 0.0f)
		return  false;

	if(Delta < 0.0f) {
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	const float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if(ActualDelta < 0.0f && Health <= 0.0f) {
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();

		if(GM)
			GM->OnActorKilled(GetOwner(), InstigatorActor);
	}
	
	return ActualDelta != 0;
}

bool USAttributeComponent::AddRage(float Delta) {
	const float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0, MaxRage);

	const float ActualDelta = Rage - OldRage;
	OnRageChanged.Broadcast(this, Rage);
	
	return ActualDelta != 0;
}

bool USAttributeComponent::UseRage(float Delta) {
	if(Delta > Rage)
		return  false;

	Rage = FMath::Clamp(Rage - Delta, 0, MaxRage);
	OnRageChanged.Broadcast(this, Rage);
	
	return true;
}

float USAttributeComponent::GetHealth() const {
	return Health;
}

float USAttributeComponent::GetMaxHealth() const{
	return  MaxHealth;
}

