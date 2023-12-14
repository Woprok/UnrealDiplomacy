// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDGameOverViewModel.h"
#include "Core/UDSessionSubsystem.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "GameOver"

void UUDGameOverViewModel::Setup()
{
	FText gameOver = FText(LOCTEXT("GameOver", "Game Over!"));
	SetGameOverTitleText(gameOver);
	FText winnerFaction = FText(LOCTEXT("GameOver", "Winner was not decided!"));
	SetWinnerFactionText(winnerFaction);
	FText returnToMenu = FText(LOCTEXT("GameOver", "Return to Menu"));
	SetReturnToMenuText(returnToMenu);
	
	SetIsHostValue(false);
	SetIsPlayerValue(false);
	SetIsWinnerValue(false);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDGameOverViewModel::Refresh);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDGameOverViewModel::Refresh);
}

void UUDGameOverViewModel::Refresh()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	SetIsHostValue(sessions->IsLocalPlayerHost(sessions->GetSessionName()));
	// Following updates require model.
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGameOver())
		return;
	ChangeGameOverPresentation();
}

void UUDGameOverViewModel::ChangeGameOverPresentation()
{
	FUDGameOverInfo info = Model->GetGameOverInfo();

	bool isPlayer = Model->IsLocalFactionPlayer();

	FText winnerFaction;
	// Win
	if (isPlayer && info.IsWinner)
	{
		winnerFaction = FText(LOCTEXT("GameOver", "Glory to your reign!"));
		FText victory = FText(LOCTEXT("GameOver", "Victory!"));
		SetGameOverTitleText(victory);
	}
	// Lose
	if (isPlayer && !info.IsWinner)
	{
		winnerFaction = FText::Format(LOCTEXT("GameOver", "Empire was taken over by {0}"),
			FText::FromString(info.WinnerFactionName));
		FText defeat = FText(LOCTEXT("GameOver", "Defeated!"));
		SetGameOverTitleText(defeat);
	}
	// Support
	if (isPlayer && info.IsSupporter)
	{
		winnerFaction = FText::Format(LOCTEXT("GameOver", "Empire was taken over by {0}"),
			FText::FromString(info.WinnerFactionName));
		FText support = FText(LOCTEXT("GameOver", "Glory to your friend reign!"));
		SetGameOverTitleText(support);
	}
	// Observer
	if (!isPlayer)
	{
		winnerFaction = FText::Format(LOCTEXT("GameOver", "Empire was taken over by {0}"),
			FText::FromString(info.WinnerFactionName));
		FText gameOver = FText(LOCTEXT("GameOver", "Game Over!"));
		SetGameOverTitleText(gameOver);
	}

	SetIsPlayerValue(isPlayer);
	SetIsWinnerValue(info.IsWinner);

	SetWinnerFactionText(winnerFaction);
}

#undef LOCTEXT_NAMESPACE

void UUDGameOverViewModel::Return()
{
	UE_LOG(LogTemp, Log, TEXT("UUDGameOverViewModel: Return."));
	ReturnToMenu();
}

void UUDGameOverViewModel::ReturnToMenu()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	if (IsHostValue)
	{
		sessions->QuitSession();
	}
	else
	{
		sessions->LeaveSession();
	}
}

void UUDGameOverViewModel::SetGameOverTitleText(FText newGameOverTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(GameOverTitleText, newGameOverTitleText);
}

FText UUDGameOverViewModel::GetGameOverTitleText() const
{
	return GameOverTitleText;
}

void UUDGameOverViewModel::SetWinnerFactionText(FText newWinnerFactionText)
{
	UE_MVVM_SET_PROPERTY_VALUE(WinnerFactionText, newWinnerFactionText);
}

FText UUDGameOverViewModel::GetWinnerFactionText() const
{
	return WinnerFactionText;
}

void UUDGameOverViewModel::SetReturnToMenuText(FText newReturnToMenuText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ReturnToMenuText, newReturnToMenuText);
}

FText UUDGameOverViewModel::GetReturnToMenuText() const
{
	return ReturnToMenuText;
}

void UUDGameOverViewModel::SetIsHostValue(bool newIsHostValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsHostValue, newIsHostValue);
}

bool UUDGameOverViewModel::GetIsHostValue() const
{
	return IsHostValue;
}

void UUDGameOverViewModel::SetIsWinnerValue(bool newIsWinnerValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsWinnerValue, newIsWinnerValue);
}

bool UUDGameOverViewModel::GetIsWinnerValue() const
{
	return IsWinnerValue;
}

void UUDGameOverViewModel::SetIsPlayerValue(bool newIsPlayerValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsPlayerValue, newIsPlayerValue);
}

bool UUDGameOverViewModel::GetIsPlayerValue() const
{
	return IsPlayerValue;
}