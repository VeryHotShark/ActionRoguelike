// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "SCharacter.generated.h"

// class UAnimMontage;
// class UCameraComponent;
// class USpringArmComponent;
// class USAttributeComponent;
// class USInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category="VFX")
	FName TimeToHitParamName;
	
	UPROPERTY(VisibleAnywhere, Category="VFX")
	FName HandSocketName = "Muzzle_01";
	
	UPROPERTY(EditDefaultsOnly, Category="VFX")
	UParticleSystem* MuzzleVFX;
	
	UPROPERTY(EditAnywhere, Category="Ability")
	TSubclassOf<AActor> AbilityProjectile;
	
	UPROPERTY(EditAnywhere, Category="Ability")
	UAnimMontage* AbilityAnim;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> PrimaryProjectile;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> SecondaryProjectile;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* SecondaryAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float AttackDelay = 0.2f;
	
	FTimerHandle TimerHandle_FirstAbility;
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SecondaryAttack;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:


	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void FirstAbility();
	void PrimaryInteract();
	void PrimaryAttack();
	void SecondaryAttack();
	void FirstAbility_TimeElapsed();
	void PrimaryAttack_TimeElapsed();
	void SecondaryAttack_TimeElapsed();

	bool SweepFromCamera(FHitResult& HitResult);
	void SpawnProjectile(TSubclassOf<AActor> Projectile,FVector Location,bool Hit,const FHitResult& HitResult );

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
