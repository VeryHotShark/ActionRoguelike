// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Components/CapsuleComponent.h"
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
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	TimeToHitParamName = "TimeToHit";
	bUseControllerRotationYaw = false;
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
	if (Delta < 0.0f) {
		SkeletalMeshComp->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		AttributeComp->AddRage(FMath::Abs(Delta));	
	}

	if (NewHealth <= 0.0f && Delta < 0.0f) {
		APlayerController* PC = Cast<APlayerController>(GetController());
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DisableInput(PC);
	}
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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
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

void ASCharacter::SprintStart() {
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop() {
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::FirstAbility() {
	ActionComp->StartActionByName(this, "PrimaryAbility");
}

void ASCharacter::PrimaryAttack() {
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::SecondaryAttack() {
	ActionComp->StartActionByName(this, "SecondaryAttack");
}

void ASCharacter::HealSelf(float Amount /* = 100 */) {
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::PrimaryInteract() {
	if (InteractionComp)
		InteractionComp->PrimaryInteract();
}

FVector ASCharacter::GetPawnViewLocation() const {
	return CameraComp->GetComponentLocation();
}

