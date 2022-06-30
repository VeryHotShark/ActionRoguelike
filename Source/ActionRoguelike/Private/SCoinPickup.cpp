// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoinPickup.h"

#include "SPlayerState.h"

class ASPlayerState;

// Sets default values
ASCoinPickup::ASCoinPickup()
{
	CreditsAmount = 10;	
}

void ASCoinPickup::Interact_Implementation(APawn* InstigatorPawn) {
	ASPlayerState* PlayerState = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());

	if(PlayerState) {
		PlayerState->GrantCredits(CreditsAmount);
		Super::Interact_Implementation(InstigatorPawn);	
	}
}

