// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack() {
	AttackDelay = 0.2f;
	HandSocketName = "Muzzle_01";
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if(Character) {
		Character->PlayAnimMontage(AttackAnim);
		
		UGameplayStatics::SpawnEmitterAttached
			(MuzzleVFX,
			Character->GetMesh(),
			HandSocketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_AttackDelay,
			Delegate,
			AttackDelay,
			false);
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter) {
	if(!ensureAlways(ProjectileClass))
		return;
	
	FCollisionShape Shape;
	Shape.SetSphere(20.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorCharacter);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector CameraLocation = InstigatorCharacter->GetPawnViewLocation();
	FVector EndLocation = CameraLocation + InstigatorCharacter->GetControlRotation().Vector() * 10000.0f;

	FHitResult HitResult;
	bool Hit = GetWorld()->SweepSingleByObjectType(
		HitResult,
		CameraLocation,
		EndLocation,
		FQuat::Identity,
		ObjectQueryParams,
		Shape,
		Params);
	
	FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

	FRotator RotationToTarget = FRotationMatrix::MakeFromX(
		(Hit ? HitResult.ImpactPoint : HitResult.TraceEnd) - HandLocation).Rotator();
	FTransform SpawnTM = FTransform(RotationToTarget, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = InstigatorCharacter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

	StopAction(InstigatorCharacter);
}

