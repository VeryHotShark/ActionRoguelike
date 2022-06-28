// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if(ensure(BlackboardComp)) {
		APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
		USAttributeComponent* AIAttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

		bool bIsLowHealth = AIAttributeComp ? AIAttributeComp->GetHealth() <= Threshold : false;

		BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
	}
}
