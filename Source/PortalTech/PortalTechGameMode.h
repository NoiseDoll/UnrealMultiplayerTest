// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortalTechGameMode.generated.h"

class APTAIController;
class APTGameStateBase;
class AToy;

UCLASS(minimalapi)
class APortalTechGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<APTAIController*> AIs;

	UPROPERTY()
	APTGameStateBase* PTGameState;

	float CurrentTime;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
	float GameTime;

public:
	APortalTechGameMode();

public:
	virtual void InitGameState() override;

	virtual void Tick(float DeltaSeconds) override;

	void RegisterAI(APTAIController* Controller);

	void OnToyThrown(AToy* Toy);
	void OnToyGrabbed(APTAIController* Controller, AToy* Toy);

	void StopAll();

	UFUNCTION()
	void ToyLanded(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};



