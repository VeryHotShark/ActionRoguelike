#include "SPotion.h"

void ASPotion::Interact_Implementation(APawn* InstigatorPawn) {
	UE_LOG(LogTemp,Display,TEXT("DUPA"));
}

// Sets default values
ASPotion::ASPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
}

void ASPotion::EnableMesh(bool State) {
	MeshComp->SetVisibility(State);
	SetActorEnableCollision(State);
}


