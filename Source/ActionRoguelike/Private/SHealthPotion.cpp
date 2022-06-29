// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion() {
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
	// Super::Interact_Implementation(InstigatorPawn);
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if(!AttributeComp || AttributeComp->IsMaxHealth())
		return;
    
    if(AttributeComp->ApplyHealthChange(this, AttributeComp->GetMaxHealth()))
		HideAndCooldownPowerup();
}


