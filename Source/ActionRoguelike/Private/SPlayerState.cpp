// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

void ASPlayerState::OnRep_Credits(int OldCredits) {
	OnCreditsChanged.Broadcast(Credits);
}

int ASPlayerState::GetCredits() const {
	return Credits;
}

void ASPlayerState::GrantCredits(int Count) {
	if(Count > 0) {
		Credits += Count;
		OnCreditsChanged.Broadcast(Credits);	
	}
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject) {
	if(SaveObject) {
		SaveObject->SavedCredits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject) {
	if(SaveObject) {
		// Credits = SaveObject->SavedCredits;
		GrantCredits(SaveObject->SavedCredits);
	}
}

bool ASPlayerState::UseCredits(int Count) {
	if(Credits >= Count) {
		Credits -= Count;
		OnCreditsChanged.Broadcast(Credits);
		return true;
	}

	return false;
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}
