// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsChanged, float, NewCredits);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category="Credits")
	int Credits;

	UFUNCTION()
	void OnRep_Credits(int OldCredits);

public:
	UFUNCTION(BlueprintCallable, Category="Credits")
	int GetCredits() const;
	
	UFUNCTION(BlueprintCallable, Category="Credits")
	bool UseCredits(int Count);
	
	UFUNCTION(BlueprintCallable, Category="Credits")
	void GrantCredits(int Count);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);
};
