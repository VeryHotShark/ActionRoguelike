// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTask_HealMax.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealMax::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OwnerComp.GetAIOwner()->GetPawn()->GetComponentByClass(USAttributeComponent::StaticClass()));

	if(AttributeComp) {
		bool bHealSuccess = AttributeComp->ApplyHealthChange(AttributeComp->GetMaxHealth());
		return bHealSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	return  EBTNodeResult::Failed;
}
