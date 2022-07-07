// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USActionComponent();

protected:
	UFUNCTION(Server,Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server,Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);
	
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	UPROPERTY(Replicated)
	TArray<USAction*> Actions;
	
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool HasAction(TSubclassOf<USAction> ActionClass);
	
	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);
	
	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);
	
	UFUNCTION(BlueprintCallable, Category="Actions")
	void RemoveAction(USAction* ActionToRemove);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;	
};
