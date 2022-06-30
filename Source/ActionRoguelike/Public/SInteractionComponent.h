// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWorldUserWidget.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent {
	GENERATED_BODY()

public:
	USInteractionComponent();

protected:

	void FindBestInteractable();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(EditDefaultsOnly, Category="Interact")
	float InteractDistance = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category="Interact")
	float InteractRadius = 30.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Interact")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY()
	AActor* FocusedActor;

	void PrimaryInteract();
};
