// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	AttackDelay = 0.2f;
	bUseControllerRotationYaw = false;
	TimeToHitParamName = "TimeToHit";
	HandSocketName = "Muzzle_01";
}


void ASCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::BeginPlay() {
	Super::BeginPlay();

	SkeletalMeshComp = GetMesh();
}


void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                  float Delta) {

	if(Delta < 0.0f) 
		SkeletalMeshComp->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
	if (NewHealth <= 0.0f && Delta < 0.0f) {
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ASCharacter::HealSelf(float Amount /* = 100 */) {
	AttributeComp->ApplyHealthChange(this, Amount);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("FirstAbility", IE_Pressed, this, &ASCharacter::FirstAbility);
}

void ASCharacter::MoveForward(float Value) {
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value) {
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::FirstAbility() {
	PlayAnimMontage(AbilityAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_FirstAbility, this, &ASCharacter::FirstAbility_TimeElapsed, AttackDelay);
}


void ASCharacter::PrimaryAttack() {
	PlayAnimMontage(AttackAnim);
	
	UGameplayStatics::SpawnEmitterAttached
		(MuzzleVFX,
		GetMesh(),
		HandSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackDelay);
}

void ASCharacter::SecondaryAttack() {
	PlayAnimMontage(SecondaryAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, AttackDelay);
}

void ASCharacter::FirstAbility_TimeElapsed() {
	FHitResult HitResult;
	bool Hit = SweepFromCamera(HitResult);

	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
	SpawnProjectile(AbilityProjectile, HandLocation, Hit, HitResult);
}


void ASCharacter::PrimaryAttack_TimeElapsed() {
	FHitResult HitResult;
	bool Hit = SweepFromCamera(HitResult);

	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
	SpawnProjectile(PrimaryProjectile, HandLocation, Hit, HitResult);
}

void ASCharacter::SecondaryAttack_TimeElapsed() {
	FHitResult HitResult;
	bool Hit = SweepFromCamera(HitResult);

	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_02");
	SpawnProjectile(SecondaryProjectile, HandLocation, Hit, HitResult);
}

bool ASCharacter::SweepFromCamera(FHitResult& HitResult) {
	FCollisionShape Shape;
	Shape.SetSphere(20.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector EndLocation = CameraLocation + CameraComp->GetForwardVector() * 10000.0f;

	return GetWorld()->SweepSingleByObjectType(
		HitResult,
		CameraLocation,
		EndLocation,
		FQuat::Identity,
		ObjectQueryParams,
		Shape,
		Params);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> Projectile, FVector Location, bool Hit,
                                  const FHitResult& HitResult) {
	// FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(Location, Hit ? HitResult.ImpactPoint : HitResult.TraceEnd);
	FRotator RotationToTarget = FRotationMatrix::MakeFromX(
		(Hit ? HitResult.ImpactPoint : HitResult.TraceEnd) - Location).Rotator();
	FTransform SpawnTM = FTransform(RotationToTarget, Location);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(Projectile, SpawnTM, SpawnParams);
}

void ASCharacter::PrimaryInteract() {
	if (InteractionComp)
		InteractionComp->PrimaryInteract();
}
