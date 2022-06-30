// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAICharacter.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable Spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase() {
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 10;
	PickupsCount = 10;
}

void ASGameModeBase::StartPlay() {
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
	
	UEnvQueryInstanceBlueprintWrapper* PickupQueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPickupsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

	if(ensure(PickupQueryInstance))
		PickupQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPickupQueryCompleted);
}

void ASGameModeBase::SpawnBotTimerElapsed() {
	if(!CVarSpawnBots.GetValueOnGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning disabled via cvar 'CvarSpawnBots'."));
		return;
	}
	
	int32 AliveBotsCount = 0;

	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);

		if(AttributeComp && AttributeComp->IsAlive()) {
			
			AliveBotsCount++;
		}
	}
	
	float MaxBotCount = 10;
	
	if(DifficultyCurve) 
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);

	if(AliveBotsCount >= MaxBotCount)
		return;
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if(ensure(QueryInstance))
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                      EEnvQueryStatus::Type QueryStatus) {
	if(QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed"));	
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if(Locations.Num() > 0 && Locations.IsValidIndex(0)) 
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator, Params);
}

void ASGameModeBase::OnPickupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus) {

	if(QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Pickups EQS Query Failed"));	
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if(Locations.Num() > 0) {
		for (int i = 0; i < PickupsCount; i ++) {
			int RandIndex = FMath::RandRange(0, Locations.Num() - 1);
			
			if(Locations.IsValidIndex(RandIndex)) {
				TSubclassOf<AActor> RandomPickup = FMath::RandBool() ? CoinClass : PotionClass;
				GetWorld()->SpawnActor<AActor>(RandomPickup, Locations[RandIndex], FRotator::ZeroRotator, Params);
				Locations.RemoveAt(RandIndex);
			}
		}
	}
}


void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer) {
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);

	if(Player) {
		float RespawnDelay = 2.0f;
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
	else {
		ASCharacter* PlayerKiller = Cast<ASCharacter>(Killer);
		
		if(PlayerKiller) {
			ASPlayerState* PlayerState = Cast<ASPlayerState>(PlayerKiller->GetPlayerState());
			PlayerState->GrantCredits(CreditsPerKill);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller) {
	if(ensure(Controller)) {
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::KillAll() {
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);

		if(AttributeComp && AttributeComp->IsAlive()) 
			AttributeComp->Kill(this);			
	}
}
