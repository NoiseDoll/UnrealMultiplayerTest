// Fill out your copyright notice in the Description page of Project Settings.


#include "PTGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "PTAICharacter.h"

APTGameStateBase::APTGameStateBase()
{
	bIsToyAvailable = true;
	RemainingSecond = 0;
	bIsGameOver = false;
}

void APTGameStateBase::SetIsToyAvailable(bool Value)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (bIsToyAvailable != Value)
		{
			bIsToyAvailable = Value;
			ToyAvailableChanged.Broadcast(bIsToyAvailable);
		}
	}
}

void APTGameStateBase::AddAI(APTAICharacter* AI)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Scores.Add({ AI, 0});
		ScoreChanged.Broadcast(Scores);
	}
}

void APTGameStateBase::AddAIScore(APTAICharacter* AI, int Score)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		for (auto& Record : Scores)
		{
			if (Record.AI == AI)
			{
				Record.Score += Score;
			}
		}

		ScoreChanged.Broadcast(Scores);
	}
}

void APTGameStateBase::SetRemainingTime(int Seconds)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (RemainingSecond != Seconds)
		{
			RemainingSecond = Seconds;
			TimeChanged.Broadcast(RemainingSecond);
		}
	}
}

void APTGameStateBase::SetIsGameOver(bool Value)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (bIsGameOver != Value)
		{
			bIsGameOver = Value;
			IsGameOverChanged.Broadcast(bIsGameOver);
		}
	}
}

void APTGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APTGameStateBase, bIsToyAvailable);
	DOREPLIFETIME(APTGameStateBase, Scores);
	DOREPLIFETIME(APTGameStateBase, RemainingSecond);
	DOREPLIFETIME(APTGameStateBase, bIsGameOver);
}

void APTGameStateBase::OnRep_IsToyAvailable()
{
	ToyAvailableChanged.Broadcast(bIsToyAvailable);
}

void APTGameStateBase::OnRep_Scores()
{
	ScoreChanged.Broadcast(Scores);
}

void APTGameStateBase::OnRep_RemainingSeconds()
{
	TimeChanged.Broadcast(RemainingSecond);
}

void APTGameStateBase::OnRep_IsGameOver()
{
	IsGameOverChanged.Broadcast(bIsGameOver);
}
