// Fill out your copyright notice in the Description page of Project Settings.


#include "PTHUD.h"
#include "PTAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PortalTechPlayerController.h"

void APTHUD::BeginPlay()
{
	Super::BeginPlay();

	auto* GameState = GetWorld()->GetGameState<APTGameStateBase>();
	GameState->ScoreChanged.AddUObject(this, &ThisClass::OnScoreChanged);
	GameState->TimeChanged.AddUObject(this, &ThisClass::OnTimeChanged);
	GameState->IsGameOverChanged.AddUObject(this, &ThisClass::OnIsGameOverChanged);

	TArray<FScoreRecord> Scores;
	GameState->GetScores(Scores);
	OnScoreChanged(Scores);

	OnTimeChanged(GameState->GetRemainingSeconds());
}

void APTHUD::OnScoreChanged(TArray<FScoreRecord>& Scores)
{
	int MaxScore = 0;
	for (auto& Record : Scores)
	{
		if (AddedAIs.Contains(Record.AI))
		{
			UpdateScore(Record.AI, Record.Score);
		}
		else
		{
			AddedAIs.Add(Record.AI);
			AddRecord(Record.AI);
		}

		if (Record.Score > MaxScore)
		{
			MaxScore = Record.Score;
		}
	}

	ClearLeaders();
	for (auto& Record : Scores)
	{
		if (Record.Score == MaxScore)
		{
			SetLeader(Record.AI);
		}
	}
}

void APTHUD::OnTimeChanged(int Seconds)
{
	UpdateTime(Seconds);
}

void APTHUD::OnIsGameOverChanged(bool Value)
{
	if (Value)
	{
		auto* GameState = GetWorld()->GetGameState<APTGameStateBase>();
		TArray<FScoreRecord> Scores;
		GameState->GetScores(Scores);

		int MaxScore = 0;
		int Count = 0;
		FString Name;
		for (auto& Record : Scores)
		{
			if (Record.Score > MaxScore)
			{
				Count = 1;
				MaxScore = Record.Score;
				Name = Record.AI->GetName();
			}
			else if (Record.Score == MaxScore)
			{
				Count++;
			}
		}

		if (Count > 1)
		{
			SetGameOver(TEXT("Draw"), Value);
		}
		else
		{
			SetGameOver(FString::Printf(TEXT("%s: %d"), *Name, MaxScore), Value);
		}
	}
	else
	{
		SetGameOver("", Value);
	}
}

void APTHUD::OnRestartClicked()
{
	auto* PC = Cast<APortalTechPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PC->ServerRestart();

}

void APTHUD::OnExitClicked()
{
	auto* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->ConsoleCommand("quit");
}
