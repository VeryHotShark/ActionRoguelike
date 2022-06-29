// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.DebugDrawInteraction"), true, TEXT("Enable Debug Draw on Interaction Component."), ECVF_Cheat);


void USInteractionComponent::PrimaryInteract() {
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + EyeRotation.Vector() * InteractDistance;

	// FHitResult HitResult;
	// bool Hit = GetWorld()->LineTraceSingleByObjectType(HitResult, EyeLocation, End, ObjectQueryParams);
	
	TArray<FHitResult> HitResults;
	float Radius = 30.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool Hit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor DebugColor = Hit ?FColor::Green : FColor::Red;

	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	for (FHitResult HitResult : HitResults) {
		AActor* HitActor = HitResult.GetActor();

		if (HitActor && HitActor->Implements<USGameplayInterface>()) {
			APawn* MyPawn = Cast<APawn>(MyOwner);
			ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
			
			if(bDebugDraw)
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Radius, 10, DebugColor, false, 2);
			break;
		}
	}

	if(bDebugDraw)
		DrawDebugLine(GetWorld(), EyeLocation,End,DebugColor, false, 2);
}
