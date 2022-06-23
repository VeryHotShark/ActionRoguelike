// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include <string>

#include "SAttributeComponent.h"
#include "SMagicProjectile.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(StaticMeshComp);
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->Radius = 700.0f;
	RadialForceComp->SetAutoActivate(false);
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASExplosiveBarrel::PostInitializeComponents() {
	Super::PostInitializeComponents();
	
	StaticMeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnHit);
}

void ASExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	ASMagicProjectile* Projectile = Cast<ASMagicProjectile>(OtherActor);
	if(Projectile != nullptr) {
		StaticMeshComp->WakeAllRigidBodies();	
		RadialForceComp->FireImpulse();

		if(OtherActor && OtherActor->GetInstigator() != nullptr) {
			float Dist = FVector::Distance(OtherActor->GetActorLocation(), GetActorLocation());
			
			if(Dist < RadialForceComp->Radius) {
				USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetInstigator()->GetComponentByClass(USAttributeComponent::StaticClass()));

				if(AttributeComp) 
					AttributeComp->ApplyHealthChange(-50.0f);
			}
		}

		Destroy();
	}
}



