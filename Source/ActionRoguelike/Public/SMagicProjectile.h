// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	ASMagicProjectile();
	
protected:
	virtual void BeginPlay() override;
	virtual void Explode_Implementation() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float DamageAmount = -20.0f;

	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTag ParryTag;

	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<UCameraShakeBase> Shake;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
