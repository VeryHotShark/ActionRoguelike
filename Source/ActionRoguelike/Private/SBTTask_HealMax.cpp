// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTask_HealMax.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealMax::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());

	if(!MyPawn)
		return EBTNodeResult::Failed;
	
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);

	if(ensure(AttributeComp)) 
		AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetMaxHealth());
	
	return  EBTNodeResult::Succeeded;
}
