// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.DebugDrawInteraction"), true, TEXT("Enable Debug Draw on Interaction Component."), ECVF_Cheat);

USInteractionComponent::USInteractionComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	InteractRadius = 30.0f;
	InteractDistance = 400.0f;
	CollisionChannel = ECC_WorldDynamic;
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable() {
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + EyeRotation.Vector() * InteractDistance;
	
	TArray<FHitResult> HitResults;
	FCollisionShape Shape;
	Shape.SetSphere(InteractRadius);

	bool Hit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor DebugColor = Hit ?FColor::Green : FColor::Red;

	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FocusedActor = nullptr;

	for (FHitResult HitResult : HitResults) {
		
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->Implements<USGameplayInterface>()) {
			FocusedActor = HitActor;
			
			if(bDebugDraw)
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, InteractRadius, 10, DebugColor, false, 2);
			
			break;
		}
	}

	if(FocusedActor) {
		if(DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);

		if(DefaultWidgetInstance) {
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if(!DefaultWidgetInstance->IsInViewport())
				DefaultWidgetInstance->AddToViewport();
		}
	}
	else {
		if(DefaultWidgetInstance)
			DefaultWidgetInstance->RemoveFromParent();
	}

	if(bDebugDraw)
		DrawDebugLine(GetWorld(), EyeLocation,End,DebugColor, false, 2);
}

void USInteractionComponent::PrimaryInteract() {
	if(FocusedActor == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to Interact");
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}
