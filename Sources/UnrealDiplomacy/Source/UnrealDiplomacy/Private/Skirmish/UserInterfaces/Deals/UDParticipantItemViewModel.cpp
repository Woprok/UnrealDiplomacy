// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDParticipantItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"

#define LOCTEXT_NAMESPACE "ParticipantItem"

void UUDParticipantItemViewModel::Initialize()
{
	FText factionName = FText(LOCTEXT("ParticipantItem", "Faction"));
	SetFactionNameText(factionName);
	FText readyState = FText(LOCTEXT("ParticipantItem", "?"));
	SetReadyStateText(readyState);
	FText voteState = FText(LOCTEXT("ParticipantItem", "?"));
	SetVoteStateText(voteState);
}

void UUDParticipantItemViewModel::Update()
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
}

#undef LOCTEXT_NAMESPACE

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