#include "SActionComponent.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

USActionComponent::USActionComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay() {
	Super::BeginPlay();

	// Server Only
	if(GetOwner() -> HasAuthority()) {
		for (TSubclassOf<USAction> ActionClass : DefaultActions)
			AddAction(GetOwner(),ActionClass);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw All Actions
	for(USAction* Action : Actions) {
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action %s" ),
			*GetNameSafe(GetOwner()),
			*GetNameSafe(Action));

		LogOnScreen(this,ActionMsg, TextColor, 0.0f);
	}
}

bool USActionComponent::HasAction(TSubclassOf<USAction> ActionClass) {
	// FName ActionName = ActionClass.GetDefaultObject()->ActionName;

	for (USAction* Action : Actions) {
		if (Action && Action->IsA(ActionClass)) 
			return true;
	}

	return false;
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass) {
	if (!ensure(ActionClass))
		return;

	USAction* NewAction = NewObject<USAction>(this, ActionClass);

	if (ensure(NewAction)) {
		NewAction->Initialize(this);
		
		Actions.Add(NewAction);

		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
			NewAction->StartAction(Instigator);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName) {
	for (USAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if(!Action->CanStart(Instigator)) {
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			// Is Client?
			if(!GetOwner()->HasAuthority())
				ServerStartAction(Instigator, ActionName);
			
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName) {
	for (USAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if(Action->IsRunning()) {
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void USActionComponent::RemoveAction(USAction* ActionToRemove) {
	if(!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
		return;
	
	Actions.Remove(ActionToRemove);
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName) {
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) {
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (USAction* Action : Actions) {
		if(Action)
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
	}
	
	return WroteSomething;
}
