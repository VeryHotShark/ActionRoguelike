// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

void ASTeleportProjectile::BeginPlay() {
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASTeleportProjectile::Explode, DetonateDelay);
}

void ASTeleportProjectile::Explode_Implementation() {
	EffectComp->Deactivate();
	MovementComp->StopMovementImmediately();
	
	SetActorEnableCollision(false);
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASTeleportProjectile::TeleportInstigator, TeleportDelay);
}

void ASTeleportProjectile::TeleportInstigator() {
	AActor* ActorToTeleport = GetInstigator();

	if(ensure(ActorToTeleport)) {
		FRotator Rotation = GetActorRotation();
		Rotation.Roll = 0;
		Rotation.Pitch = 0;
		ActorToTeleport->TeleportTo(GetActorLocation(), Rotation, false,true);
		Destroy();
	}
}


