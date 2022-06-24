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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
