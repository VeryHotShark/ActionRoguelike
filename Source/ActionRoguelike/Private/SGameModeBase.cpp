// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAICharacter.h"
#include "SAttributeComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ASGameModeBase::ASGameModeBase() {
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay() {
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll() {
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);

		if(AttributeComp && AttributeComp->IsAlive()) 
			AttributeComp->Kill(this);			
	}
}

void ASGameModeBase::SpawnBotTimerElapsed() {
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
