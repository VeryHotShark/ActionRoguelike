// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile() {
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);
}

void ASMagicProjectile::PostInitializeComponents() {
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::BeginPlay() {
	Super::BeginPlay();

	AudioComp->Play();
}

void ASMagicProjectile::Explode_Implementation() {
	UGameplayStatics::PlayWorldCameraShake(this,Shake,GetActorLocation(),0,2500);
	
	Super::Explode_Implementation();	
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if(OtherActor && OtherActor != GetInstigator()) {
		// USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		//
		// if(AttributeComp)
		// 	AttributeComp->ApplyHealthChange(GetInstigator(), DamageAmount);
		//
		// Explode();

		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult);
		Explode();
	}
}
