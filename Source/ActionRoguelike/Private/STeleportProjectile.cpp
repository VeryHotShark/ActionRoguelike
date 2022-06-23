// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

void ASTeleportProjectile::BeginPlay() {
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Lifetime, this, &ASTeleportProjectile::Teleport, 1.0f);
}


void ASTeleportProjectile::PostInitializeComponents() {
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::ASTeleportProjectile::OnHit);
}

void ASTeleportProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	GetWorldTimerManager().ClearTimer(TimerHandle_Lifetime);
	DrawDebugString(GetWorld(), GetActorLocation(), TEXT("DUPA"), nullptr, FColor::Green, 2);
	Teleport();
}

void ASTeleportProjectile::Teleport() {
	EffectComp->Deactivate();
	MovementComp->StopMovementImmediately();
	FTransform SpawnTM = FTransform(GetActorRotation(),GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyVFX, SpawnTM);
	GetWorldTimerManager().SetTimer(TimerHandle_DestroyParticle, this, &ASTeleportProjectile::Teleport_TimeElapsed, 0.2f);
}

void ASTeleportProjectile::Teleport_TimeElapsed() {
	FRotator Rotation = GetActorRotation();
	Rotation.Pitch = 0;
	Rotation.Roll = 0;
	GetInstigator()->TeleportTo(GetActorLocation(), Rotation, false,true);
	Destroy();
}


