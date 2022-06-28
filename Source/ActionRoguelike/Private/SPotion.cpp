#include "SPotion.h"

void ASPotion::Interact_Implementation(APawn* InstigatorPawn) {
	// logic in derived classses
}

// Sets default values
ASPotion::ASPotion() {
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComp;
	RespawnDelay = 10.0f;
}

void ASPotion::HideAndCooldownPowerup() {
	SetPowerUpState(false);
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ASPotion::SetPowerUpState, true);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, RespawnDelegate, RespawnDelay, false);
}

void ASPotion::SetPowerUpState(bool State) {
	SetActorEnableCollision(State);
	RootComponent->SetVisibility(State, true);
}
