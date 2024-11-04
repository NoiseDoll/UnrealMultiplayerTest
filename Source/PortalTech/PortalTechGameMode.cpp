// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalTechGameMode.h"
#include "PortalTechCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "PTGameStateBase.h"
#include "PTAIController.h"
#include "PTAICharacter.h"
#include <NavigationSystem.h>
#include "Toy.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "PortalTechPlayerController.h"

APortalTechGameMode::APortalTechGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));

	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PrimaryActorTick.bCanEverTick = true;
	GameTime = 60;
	CurrentTime = 0;
}

void APortalTechGameMode::InitGameState()
{
	Super::InitGameState();

	PTGameState = Cast<APTGameStateBase>(GameState);
}

void APortalTechGameMode::Tick(float DeltaSeconds)
{
	if (CurrentTime >= GameTime)
	{
		PTGameState->SetIsGameOver(true);
		StopAll();
	}
	else
	{
		CurrentTime += DeltaSeconds;
		PTGameState->SetRemainingTime(FMath::CeilToInt(GameTime - CurrentTime));
	}
}

void APortalTechGameMode::RegisterAI(APTAIController* Controller)
{
	AIs.Emplace(Controller);
	PTGameState->AddAI(Controller->GetPawn<APTAICharacter>());
}

void APortalTechGameMode::OnToyThrown(AToy* Toy)
{
	PTGameState->SetIsToyAvailable(false);

	// Calc path only at the moment when a toy landed on the ground
	Toy->OnActorHit.AddDynamic(this, &ThisClass::ToyLanded);
}

void APortalTechGameMode::OnToyGrabbed(APTAIController* Controller, AToy* Toy)
{
	PTGameState->SetIsToyAvailable(true);
	PTGameState->AddAIScore(Controller->GetPawn<APTAICharacter>(), Toy->GetScore());

	for (auto* AI : AIs)
	{
		AI->ClearToyTarget();
	}
}

void APortalTechGameMode::StopAll()
{
	SetActorTickEnabled(false);

	for (auto Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		auto* PlayerActor = Cast<APortalTechPlayerController>(Iterator->Get());
		PlayerActor->ClientDisableInput();
	}

	for (auto* AI : AIs)
	{
		AI->ClearToyTarget();
	}
}

void APortalTechGameMode::ToyLanded(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	auto* Toy = Cast<AToy>(SelfActor);
	Toy->OnActorHit.RemoveDynamic(this, &ThisClass::ToyLanded);

	AAIController* SlowestAI = nullptr;
	float time = 0;

	for (auto* AI : AIs)
	{
		auto* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
		auto* path = navSys->FindPathToLocationSynchronously(
			GetWorld(), AI->GetNavAgentLocation(), Toy->GetActorLocation(), AI);

		float estimatedTime = path->GetPathLength() / AI->GetSpeed();
		if (estimatedTime > time)
		{
			SlowestAI = AI;
			time = estimatedTime;
		}
	}

	for (auto* AI : AIs)
	{
		if (AI != SlowestAI)
		{
			AI->SetToyTarget(Toy);
		}
	}
}
