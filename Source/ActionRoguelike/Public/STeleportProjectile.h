// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UParticleSystem* DestroyVFX;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle TimerHandle_Lifetime;
	FTimerHandle TimerHandle_DestroyParticle;
	
	void Teleport();
	void Teleport_TimeElapsed();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
};
