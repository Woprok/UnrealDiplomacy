// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDParticipantItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDDealActionParticipantKick.h"
#include "Core/Simulation/UDActionData.h"
#include "Skirmish/UDSkirmishHUD.h"

#define LOCTEXT_NAMESPACE "ParticipantItem"

void UUDParticipantItemViewModel::Setup()
{
	FText factionName = FText(LOCTEXT("ParticipantItem", "Faction"));
	SetFactionNameText(factionName);
	FText readyState = FText(LOCTEXT("ParticipantItem", "?"));
	SetReadyStateText(readyState);
	FText voteState = FText(LOCTEXT("ParticipantItem", "?"));
	SetVoteStateText(voteState);
	FText kick = FText(LOCTEXT("ParticipantItem", "Kick"));
	SetKickText(kick);
	SetCanKickValue(false);
}

void UUDParticipantItemViewModel::Refresh()
{
	SetFactionNameText(FText::FromString(Content.FactionName));
	if (Content.IsReady)
	{
		FText readyState = FText(LOCTEXT("ParticipantItem", "Yes"));
		SetReadyStateText(readyState);
	}
	else
	{
		FText readyState = FText(LOCTEXT("ParticipantItem", "No"));
		SetReadyStateText(readyState);
	}
	if (Content.IsYesVote)
	{
		FText voteState = FText(LOCTEXT("ParticipantItem", "Yes"));
		SetVoteStateText(voteState);
	}
	else
	{
		FText voteState = FText(LOCTEXT("ParticipantItem", "No"));
		SetVoteStateText(voteState);
	}
	SetCanKickValue(Content.CanBeKicked);
}

#undef LOCTEXT_NAMESPACE

void UUDParticipantItemViewModel::Kick()
{
	UE_LOG(LogTemp, Log, TEXT("UUDParticipantItemViewModel: Kick."));
	FUDActionData action = Model->GetAction(UUDDealActionParticipantKick::ActionTypeId, { Content.DealId, Content.FactionId });
	Model->RequestAction(action);
}

void UUDParticipantItemViewModel::SetContent(FUDDealFactionInfo content)
{
	Content = content;
}

void UUDParticipantItemViewModel::SetFactionNameText(FText newFactionNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionNameText, newFactionNameText);
}

FText UUDParticipantItemViewModel::GetFactionNameText() const
{
	return FactionNameText;
}

void UUDParticipantItemViewModel::SetReadyStateText(FText newReadyStateText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ReadyStateText, newReadyStateText);
}

FText UUDParticipantItemViewModel::GetReadyStateText() const
{
	return ReadyStateText;
}

void UUDParticipantItemViewModel::SetVoteStateText(FText newVoteStateText)
{
	UE_MVVM_SET_PROPERTY_VALUE(VoteStateText, newVoteStateText);
}

FText UUDParticipantItemViewModel::GetVoteStateText() const
{
	return VoteStateText;
}

void UUDParticipantItemViewModel::SetKickText(FText newKickText)
{
	UE_MVVM_SET_PROPERTY_VALUE(KickText, newKickText);
}

FText UUDParticipantItemViewModel::GetKickText() const
{
	return KickText;
}

void UUDParticipantItemViewModel::SetCanKickValue(bool newCanKickValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanKickValue, newCanKickValue);
}

bool UUDParticipantItemViewModel::GetCanKickValue() const
{
	return CanKickValue;
}