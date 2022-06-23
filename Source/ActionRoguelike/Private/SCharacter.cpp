// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("LookUp",this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn",this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("FirstAbility", IE_Pressed, this, &ASCharacter::FirstAbility);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = FORWARD
	// Y = RIGHT
	// Z = UP
	
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ASCharacter::FirstAbility() {
	PlayAnimMontage(AbilityAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_FirstAbility, this, &ASCharacter::FirstAbility_TimeElapsed, 0.2f);
}


void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::SecondaryAttack() {
	PlayAnimMontage(SecondaryAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::FirstAbility_TimeElapsed() {
	FHitResult HitResult;
	bool Hit = LineTraceFromCamera(HitResult);

	FColor DebugColor = Hit ?FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), HitResult.TraceStart,Hit ? HitResult.ImpactPoint : HitResult.TraceEnd,DebugColor, false, 2);

	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	SpawnProjectile(AbilityProjectile, HandLocation,Hit, HitResult);
}


void ASCharacter::PrimaryAttack_TimeElapsed() {
	FHitResult HitResult;
	bool Hit = LineTraceFromCamera(HitResult);

	FColor DebugColor = Hit ?FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), HitResult.TraceStart,Hit ? HitResult.ImpactPoint : HitResult.TraceEnd,DebugColor, false, 2);

	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	SpawnProjectile(PrimaryProjectile, HandLocation,Hit, HitResult);
}

void ASCharacter::SecondaryAttack_TimeElapsed() {
	FHitResult HitResult;
	bool Hit = LineTraceFromCamera(HitResult);

	FColor DebugColor = Hit ?FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), HitResult.TraceStart,Hit ? HitResult.ImpactPoint : HitResult.TraceEnd,DebugColor, false, 2);
	
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_02");
	SpawnProjectile(SecondaryProjectile, HandLocation,Hit, HitResult);
}

bool ASCharacter::LineTraceFromCamera(FHitResult& HitResult) {
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector EndLocation = CameraLocation + CameraComp->GetForwardVector() * 100000.0f;
	
	return GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		CameraLocation,
		EndLocation,
		ObjectQueryParams);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> Projectile,FVector Location,bool Hit,const FHitResult& HitResult) {
	FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(Location, Hit ? HitResult.ImpactPoint : HitResult.TraceEnd);
	FTransform SpawnTM = FTransform(RotationToTarget,Location);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(Projectile, SpawnTM, SpawnParams);
}

void ASCharacter::PrimaryInteract()
{
	if(InteractionComp)
		InteractionComp->PrimaryInteract();
}

