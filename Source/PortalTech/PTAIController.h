// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PTAIController.generated.h"

class APortalTechGameMode;
class AToy;

/**
 * 
 */
UCLASS()
class PORTALTECH_API APTAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	APortalTechGameMode* GameMode;

public:
	virtual void BeginPlay() override;

	float GetSpeed() const;

	void SetToyTarget(AToy* Toy);
	void ClearToyTarget();
};
