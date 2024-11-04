// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Toy.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class APTAIController;

UCLASS(Abstract)
class PORTALTECH_API AToy : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;

public:	
	// Sets default values for this actor's properties
	AToy();

protected:

public:
	int GetScore() const { return Score; }

	UFUNCTION(BlueprintCallable)
	void GrabToy(APTAIController* Controller);
};
