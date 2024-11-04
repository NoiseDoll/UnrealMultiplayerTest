// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PTGameStateBase.generated.h"

class APTAICharacter;

USTRUCT(BlueprintType)
struct FScoreRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	APTAICharacter* AI;

	UPROPERTY(BlueprintReadOnly)
	int Score;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FValueChanged, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FScoreChanged, TArray<FScoreRecord>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FTimeChanged, int);

/**
 * 
 */
UCLASS()
class PORTALTECH_API APTGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(ReplicatedUsing = OnRep_IsToyAvailable)
	uint8 bIsToyAvailable : 1;

	UPROPERTY(ReplicatedUsing = OnRep_Scores)
	TArray<FScoreRecord> Scores;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingSeconds)
	int RemainingSecond;

	UPROPERTY(ReplicatedUsing = OnRep_IsGameOver)
	uint8 bIsGameOver : 1;
	
public:
	APTGameStateBase();

	FValueChanged ToyAvailableChanged;
	FScoreChanged ScoreChanged;
	FTimeChanged TimeChanged;
	FValueChanged IsGameOverChanged;

	bool IsToyAvailable() const { return bIsToyAvailable; }

	void GetScores(TArray<FScoreRecord>& Result) const { Result = Scores; }

	int GetRemainingSeconds() const { return RemainingSecond; }

	bool IsGameOver() const { return bIsGameOver; }

	void SetIsToyAvailable(bool Value);

	void AddAI(APTAICharacter* AI);

	void AddAIScore(APTAICharacter* AI, int Score);

	void SetRemainingTime(int Seconds);

	void SetIsGameOver(bool Value);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_IsToyAvailable();

	UFUNCTION()
	void OnRep_Scores();

	UFUNCTION()
	void OnRep_RemainingSeconds();

	UFUNCTION()
	void OnRep_IsGameOver();
};
