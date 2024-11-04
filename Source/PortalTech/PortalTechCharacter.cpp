// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalTechCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Toy.h"
#include "PTGameStateBase.h"
#include "PortalTechGameMode.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APortalTechCharacter

APortalTechCharacter::APortalTechCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GetMesh()->SetOwnerNoSee(true);

	ToyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToyMeshComp"));
	ToyMesh->SetRelativeLocation(FVector(20.f, 0.f, -10.f));
	ToyMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	ToyMesh->SetOnlyOwnerSee(true);
	ToyMesh->SetupAttachment(FirstPersonCameraComponent);
	ToyMesh->SetCollisionProfileName(TEXT("NoCollision"));

	ToyIndex = 0;
}

void APortalTechCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetWorld()->GetGameState<APTGameStateBase>();
	GameMode = GetWorld()->GetAuthGameMode<APortalTechGameMode>();
	GameState->ToyAvailableChanged.AddUObject(this, &ThisClass::OnToyAvailableChanged);
	ToyMesh->SetStaticMesh(ToyMeshes[ToyIndex]);
	ToyMesh->SetMaterial(0, ToyMaterial);
	ToyMesh->SetVisibility(GameState->IsToyAvailable());
}

//////////////////////////////////////////////////////////////////////////// Input

void APortalTechCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APortalTechCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APortalTechCharacter::Look);

		EnhancedInputComponent->BindAction(
			ThrowAction, ETriggerEvent::Triggered, this, &APortalTechCharacter::TryThrow);
		EnhancedInputComponent->BindAction(
			SwitchAction, ETriggerEvent::Triggered, this, &APortalTechCharacter::SwitchToy);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void APortalTechCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APortalTechCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APortalTechCharacter::ServerThrow_Implementation(int ClientToyIndex)
{
	// Check again on a server to avoid replication race on a client
	if (!CanThrow())
	{
		return;
	}

	FVector spawnLocation;
	FRotator spawnRotation;
	GetActorEyesViewPoint(spawnLocation, spawnRotation);

	spawnLocation += (spawnRotation.Vector() * 100.0f);

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	auto* Toy = GetWorld()->SpawnActor<AToy>(Toys[ClientToyIndex], spawnLocation, spawnRotation, spawnParameters);
	GameMode->OnToyThrown(Toy);
}

void APortalTechCharacter::TryThrow()
{
	// Check on a client to prevent overflow of reliable queue
	if (!CanThrow())
	{
		return;
	}

	ServerThrow(ToyIndex);
}

bool APortalTechCharacter::CanThrow()
{
	return GameState->IsToyAvailable();
}

void APortalTechCharacter::SwitchToy()
{
	ToyIndex = ToyIndex == 0 ? 1 : 0;
	ToyMesh->SetStaticMesh(ToyMeshes[ToyIndex]);
}

void APortalTechCharacter::OnToyAvailableChanged(bool value)
{
	ToyMesh->SetVisibility(value);
}
