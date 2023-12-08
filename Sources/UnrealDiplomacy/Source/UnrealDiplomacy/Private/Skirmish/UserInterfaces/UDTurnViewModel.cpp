// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTurnViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDSystemActionTurnFinish.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "Turn"

FText ConstructRegentToolTip()
{
	TStringBuilder<128> content;
	content.Append(FText(LOCTEXT("Turn", "Regent is temporary position that rotates between factions.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("Turn", "Regent is allowed to create new deals.")).ToString());
	return FText::FromString(content.ToString());
}

FText ConstructFinishTurnToolTip()
{
	TStringBuilder<128> content;
	content.Append(FText(LOCTEXT("Turn", "Once you are done with everything you wanted to do you need to finish turn.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("Turn", "You can still act after finishing turn as long as at least one player did not finish.")).ToString());
	return FText::FromString(content.ToString());
}

void UUDTurnViewModel::Setup()
{
	FText regentTitle = FText(LOCTEXT("Turn", "Current Regent"));
	SetRegentTitleText(regentTitle);
	FText regent = FText(LOCTEXT("Turn", "Regent Faction"));
	SetRegentText(regent);
	FText regentToolTip = ConstructRegentToolTip();
	SetRegentToolTipText(regentToolTip);
	FText remaining = FText::Format(LOCTEXT("Turn", "Waiting {0} / {1}"), 0, 0);
	SetRemainingText(remaining);
	FText finishTurn = FText(LOCTEXT("Turn", "Finish Turn"));
	SetFinishTurnText(finishTurn);
	FText finishTurnToolTip = ConstructFinishTurnToolTip();
	SetFinishTurnToolTipText(finishTurnToolTip);
	FText turn = FText::Format(LOCTEXT("Turn", "Turn {0} / {1}"), 0, 0);
	SetTurnText(turn);
	SetIsRegent(false);
	SetIsFinished(true);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDTurnViewModel::Refresh);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDTurnViewModel::Refresh);

	// TODO delete this comment if it works as expected...
	//Update();
}

void UUDTurnViewModel::Refresh()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
	UpdateTurnPresentation();
}

void UUDTurnViewModel::UpdateTurnPresentation()
{
	FUDRegencyTurnInfo info = Model->GetRegencyTurnInfo();
	FText regent = FText::FromString(info.RegentFactionName);
	SetRegentText(regent);
	SetIsRegent(info.IsRegent);

	FText remaining = FText::Format(LOCTEXT("Turn", "Waiting {0} / {1}"), info.CurrentFinished, info.MaximumFinished);
	SetRemainingText(remaining);
	FText turn = FText::Format(LOCTEXT("Turn", "Turn {0} / {1}"), info.Turn, info.MaxTurn);
	SetTurnText(turn);
	SetIsFinished(info.IsFinished);
}

#undef LOCTEXT_NAMESPACE

void UUDTurnViewModel::FinishTurn()
{
	UE_LOG(LogTemp, Log, TEXT("UUDTurnViewModel: FinishTurn."));
	Model->RequestAction(Model->GetAction(UUDSystemActionTurnFinish::ActionTypeId));
}

void UUDTurnViewModel::SetRegentTitleText(FText newRegentTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(RegentTitleText, newRegentTitleText);
}

FText UUDTurnViewModel::GetRegentTitleText() const
{
	return RegentTitleText;
}

void UUDTurnViewModel::SetRegentText(FText newRegentText)
{
	UE_MVVM_SET_PROPERTY_VALUE(RegentText, newRegentText);
}

FText UUDTurnViewModel::GetRegentText() const
{
	return RegentText;
}

void UUDTurnViewModel::SetRegentToolTipText(FText newRegentToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(RegentToolTipText, newRegentToolTipText);
}

FText UUDTurnViewModel::GetRegentToolTipText() const
{
	return RegentToolTipText;
}

void UUDTurnViewModel::SetRemainingText(FText newRemainingText)
{
	UE_MVVM_SET_PROPERTY_VALUE(RemainingText, newRemainingText);
}

FText UUDTurnViewModel::GetRemainingText() const
{
	return RemainingText;
}

void UUDTurnViewModel::SetFinishTurnText(FText newFinishTurnText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FinishTurnText, newFinishTurnText);
}

FText UUDTurnViewModel::GetFinishTurnText() const
{
	return FinishTurnText;
}

void UUDTurnViewModel::SetFinishTurnToolTipText(FText newFinishTurnToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FinishTurnToolTipText, newFinishTurnToolTipText);
}

FText UUDTurnViewModel::GetFinishTurnToolTipText() const
{
	return FinishTurnToolTipText;
}

void UUDTurnViewModel::SetTurnText(FText newTurnText)
{
	UE_MVVM_SET_PROPERTY_VALUE(TurnText, newTurnText);
}

FText UUDTurnViewModel::GetTurnText() const
{
	return TurnText;
}

void UUDTurnViewModel::SetIsRegent(bool newIsRegent)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsRegent, newIsRegent);
}

bool UUDTurnViewModel::GetIsRegent() const
{
	return IsRegent;
}

void UUDTurnViewModel::SetIsFinished(bool newIsFinished)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsFinished, newIsFinished);
}

bool UUDTurnViewModel::GetIsFinished() const
{
	return IsFinished;
}