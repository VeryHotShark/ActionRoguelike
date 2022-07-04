#include "SPotion.h"

void ASPotion::Interact_Implementation(APawn* InstigatorPawn) {
	// logic in derived classses
	HideAndCooldownPowerup();
}

// Sets default values
ASPotion::ASPotion() {
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
	
	RespawnDelay = 10.0f;

	SetReplicates(true);
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
