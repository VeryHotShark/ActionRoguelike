// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ensure(AIPawn)) {
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIPawn);

		if(ensure(AttributeComp)) {
			bool bIsLowHealth = AttributeComp->GetHealth() / AttributeComp->GetMaxHealth() <= Threshold;

			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
		}
	}
}
