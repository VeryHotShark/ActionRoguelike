// Fill out your copyright notice in the Description page of Project Settings.


#include "SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay() {
	Super::BeginPlay();

	RunBehaviorTree(BehaviourTree);

	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if(MyPawn) {
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	}
}
