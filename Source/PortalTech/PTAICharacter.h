// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PTAICharacter.generated.h"

UCLASS()
class PORTALTECH_API APTAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
	uint8 bUseRandomSpeed : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game, meta = (EditCondition = "bUseRandomSpeed"))
	float MinRandomSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game, meta = (EditCondition = "bUseRandomSpeed"))
	float MaxRandomSpeed;

public:
	// Sets default values for this character's properties
	APTAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	FString GetName() const { return Name; }
};
