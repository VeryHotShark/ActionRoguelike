// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

class USAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor ,USAttributeComponent*, OwningComp ,float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRageChanged,USAttributeComponent*, OwningComp ,float, NewRage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category= "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category= "Attributes", meta=(DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:
	UPROPERTY(VisibleAnywhere,Replicated, BlueprintReadOnly, Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly,Replicated, BlueprintReadOnly, Category="Attributes")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere,Replicated, BlueprintReadOnly, Category="Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly,Replicated, BlueprintReadOnly, Category="Attributes")
	float MaxRage;

	UFUNCTION(NetMulticast, Reliable) // @FIXME : Mark as unreliable once we moved 'state' out of scharacter
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable) // Cosmetic Changes Only
	void MulticastRageChanged(float NewRage);
	
public:
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsMaxHealth() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool AddRage(float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool UseRage(float Delta);

	virtual void BeginPlay() override;

	float GetHealth() const;	
	float GetMaxHealth() const;
};
