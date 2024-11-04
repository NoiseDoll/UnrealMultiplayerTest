// Fill out your copyright notice in the Description page of Project Settings.


#include "PTAIController.h"
#include "PortalTechGameMode.h"
#include "GameFramework/PawnMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Toy.h"
#include "PTAICharacter.h"

void APTAIController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = GetWorld()->GetAuthGameMode<APortalTechGameMode>();
	GameMode->RegisterAI(this);
}

float APTAIController::GetSpeed() const
{
	if (auto* P = GetPawn())
	{
		return P->GetMovementComponent()->GetMaxSpeed();
	}

	return 0.0f;
}

void APTAIController::SetToyTarget(AToy* Toy)
{
	GetBlackboardComponent()->SetValueAsObject("Toy", Toy);
}

void APTAIController::ClearToyTarget()
{
	GetBlackboardComponent()->ClearValue("Toy");
}
