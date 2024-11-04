// Copyright Epic Games, Inc. All Rights Reserved.


#include "PortalTechPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

void APortalTechPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void APortalTechPlayerController::ClientDisableInput_Implementation()
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
	}
}

void APortalTechPlayerController::ServerRestart_Implementation()
{
	GetWorld()->ServerTravel(TEXT("?restart"));
}
