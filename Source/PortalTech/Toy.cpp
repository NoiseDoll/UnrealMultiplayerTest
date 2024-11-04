// Fill out your copyright notice in the Description page of Project Settings.


#include "Toy.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "PortalTechGameMode.h"

// Sets default values
AToy::AToy()
{
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = Mesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = Mesh;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

void AToy::GrabToy(APTAIController* Controller)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		GetWorld()->GetAuthGameMode<APortalTechGameMode>()->OnToyGrabbed(Controller, this);
		Destroy();
	}
}

