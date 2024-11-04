// Fill out your copyright notice in the Description page of Project Settings.


#include "PTAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APTAICharacter::APTAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APTAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		if (bUseRandomSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = FMath::RandRange(MinRandomSpeed, MaxRandomSpeed);
		}
	}
}

