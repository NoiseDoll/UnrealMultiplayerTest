// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PortalTechCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class APTGameStateBase;
class APortalTechGameMode;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

class AToy;

UCLASS(config=Game)
class APortalTechCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ToyMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Throw Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;

	/** Switch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Game, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AToy>> Toys;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Game, meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMesh*> ToyMeshes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Game, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* ToyMaterial;

	int ToyIndex;
	APTGameStateBase* GameState;
	APortalTechGameMode* GameMode;
	
public:
	APortalTechCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerThrow(int ClientToyIndex);

	void TryThrow();
	bool CanThrow();
	void SwitchToy();

	void OnToyAvailableChanged(bool value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

