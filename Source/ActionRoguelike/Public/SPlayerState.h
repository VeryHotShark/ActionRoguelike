// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsChanged, float, NewCredits);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Credits")
	int Credits;

public:
	UFUNCTION(BlueprintCallable, Category="Credits")
	int GetCredits() const;
	
	UFUNCTION(BlueprintCallable, Category="Credits")
	bool UseCredits(int Count);
	
	UFUNCTION(BlueprintCallable, Category="Credits")
	void GrantCredits(int Count);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;
};
