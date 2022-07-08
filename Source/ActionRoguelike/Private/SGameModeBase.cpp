// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAICharacter.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable Spawning of bots via timer."),
                                                ECVF_Cheat);

ASGameModeBase::ASGameModeBase() {
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 10;
	PickupsCount = 10;

	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::StartPlay() {
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed,
	                                SpawnTimerInterval, true);

	UEnvQueryInstanceBlueprintWrapper* PickupQueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnPickupsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

	if (ensure(PickupQueryInstance))
		PickupQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPickupQueryCompleted);
}

void ASGameModeBase::SpawnBotTimerElapsed() {
	if (!CVarSpawnBots.GetValueOnGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning disabled via cvar 'CvarSpawnBots'."));
		return;
	}

	int32 AliveBotsCount = 0;

	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);

		if (AttributeComp && AttributeComp->IsAlive()) {

			AliveBotsCount++;
		}
	}

	float MaxBotCount = 10;

	if (DifficultyCurve)
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);

	if (AliveBotsCount >= MaxBotCount)
		return;

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                      EEnvQueryStatus::Type QueryStatus) {
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	if (Locations.Num() > 0 && Locations.IsValidIndex(0)) {
		if (MonsterTable) {
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();

			if (Manager) {
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this
					, &ASGameModeBase::OnMonsterLoaded
					, SelectedRow->MonsterId
					, Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
				// LogOnScreen(this, "Started Loading...", FColor::Yellow);
			}
		}
	}
}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation) {
	// LogOnScreen(this, "Finished Loading...", FColor::Green);
	UAssetManager* Manager = UAssetManager::GetIfValid();
	
	if(Manager) {
	
		USMonsterData* MonsterData = Cast<USMonsterData>(Manager->GetPrimaryAssetObject(LoadedID));
		
		if(MonsterData) {
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator, Params);

			if(NewBot) {
				USActionComponent* ActionComp = Cast<USActionComponent>(NewBot->GetComponentByClass(USActionComponent::StaticClass()));
				if(ActionComp) {
					for(TSubclassOf<USAction> ActionClass : MonsterData->Actions)
						ActionComp->AddAction(NewBot,ActionClass);
				}
			}
		}
	}
}

void ASGameModeBase::OnPickupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                            EEnvQueryStatus::Type QueryStatus) {

	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Pickups EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Locations.Num() > 0) {
		for (int i = 0; i < PickupsCount; i ++) {
			int RandIndex = FMath::RandRange(0, Locations.Num() - 1);

			if (Locations.IsValidIndex(RandIndex)) {
				TSubclassOf<AActor> RandomPickup = FMath::RandBool() ? CoinClass : PotionClass;
				GetWorld()->SpawnActor<AActor>(RandomPickup, Locations[RandIndex], FRotator::ZeroRotator, Params);
				Locations.RemoveAt(RandIndex);
			}
		}
	}
}


void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer) {
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);

	if (Player) {
		float RespawnDelay = 2.0f;
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
	else {
		ASCharacter* PlayerKiller = Cast<ASCharacter>(Killer);

		if (PlayerKiller) {
			if (ASPlayerState* PlayerState = PlayerKiller->GetPlayerState<ASPlayerState>())
				PlayerState->GrantCredits(CreditsPerKill);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor),
	       *GetNameSafe(Killer));
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller) {
	if (ensure(Controller)) {
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::KillAll() {
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);

		if (AttributeComp && AttributeComp->IsAlive())
			AttributeComp->Kill(this);
	}
}
