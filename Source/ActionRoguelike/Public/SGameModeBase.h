// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMonsterData.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USSaveGame;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase {
	GENERATED_BODY()

public:
	FMonsterInfoRow() {
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	// USMonsterData* MonsterData;
	// TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};

UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FString SlotName;
	
	UPROPERTY()
	USSaveGame* CurrentSaveGame;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UDataTable* MonsterTable;
	
	UPROPERTY(EditDefaultsOnly, Category="Credits")
	int CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category="Credits")
	int PickupsCount;
	
	UPROPERTY(EditDefaultsOnly, Category="Credits")
	TSubclassOf<AActor> CoinClass;

	UPROPERTY(EditDefaultsOnly, Category="Credits")
	TSubclassOf<AActor> PotionClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Credits")
	UEnvQuery* SpawnPickupsQuery;
	
	// UPROPERTY(EditDefaultsOnly, Category="AI")
	// TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnBotQuery;


	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UCurveFloat* DifficultyCurve;
	
	FTimerHandle TimerHandle_SpawnBots;

	void OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation);
	
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPickupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:
	ASGameModeBase();

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable, Category= "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
};
