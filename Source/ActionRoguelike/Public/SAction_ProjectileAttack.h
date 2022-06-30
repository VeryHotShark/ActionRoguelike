#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float AttackDelay = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	FName HandSocketName = "Muzzle_01";
	
	UPROPERTY(EditDefaultsOnly, Category="VFX")
	UParticleSystem* MuzzleVFX;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	USAction_ProjectileAttack();
};
