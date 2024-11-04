// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PTGameStateBase.h"
#include "PTHUD.generated.h"

class APTAICharacter;

/**
 * 
 */
UCLASS()
class PORTALTECH_API APTHUD : public AHUD
{
	GENERATED_BODY()

protected:
	TSet<APTAICharacter*> AddedAIs;
	
public:
	virtual void BeginPlay() override;

	void OnScoreChanged(TArray<FScoreRecord>& Scores);
	void OnTimeChanged(int Seconds);
	void OnIsGameOverChanged(bool Value);

	UFUNCTION(BlueprintImplementableEvent)
	void AddRecord(APTAICharacter* AI);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScore(APTAICharacter* AI, int Score);

	UFUNCTION(BlueprintImplementableEvent)
	void ClearLeaders();

	UFUNCTION(BlueprintImplementableEvent)
	void SetLeader(APTAICharacter* AI);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTime(int Seconds);

	UFUNCTION(BlueprintImplementableEvent)
	void SetGameOver(const FString& Message, bool IsOn);

	UFUNCTION(BlueprintCallable)
	void OnRestartClicked();

	UFUNCTION(BlueprintCallable)
	void OnExitClicked();
};
