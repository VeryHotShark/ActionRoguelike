// Fill out your copyright notice in the Description page of Project Settings.


#include "SAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ASAICharacter::ASAICharacter() {
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
}

void ASAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta) {
	
}

void ASAICharacter::OnPawnSeen(APawn* Pawn) {
	AAIController* AIC = Cast<AAIController>(GetController());

	if(AIC) {
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		BBComp->SetValueAsObject("TargetActor", Pawn);
		DrawDebugString(GetWorld(), Pawn->GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}
