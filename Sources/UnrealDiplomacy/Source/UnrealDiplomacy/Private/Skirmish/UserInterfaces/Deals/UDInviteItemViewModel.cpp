// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDInviteItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInviteConsequence.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

#define LOCTEXT_NAMESPACE "InviteItem"

void UUDInviteItemViewModel::Setup()
{
	FText factionName = FText(LOCTEXT("ParticipantItem", "Faction"));
	SetFactionNameText(factionName);
	FText invite = FText(LOCTEXT("ParticipantItem", "Invite"));
	SetInviteText(invite);
	FText blocked = FText(LOCTEXT("ParticipantItem", "Blocked"));
	SetBlockedText(blocked);
	SetIsInviteableValue(false);
}

void UUDInviteItemViewModel::Refresh()
{
	SetFactionNameText(FText::FromString(Content.FactionName));
	SetIsInviteableValue(Content.IsInviteble);
}

#undef LOCTEXT_NAMESPACE

void UUDInviteItemViewModel::Invite()
{
	UE_LOG(LogTemp, Log, TEXT("UUDInviteItemViewModel: Invite."));
	FUDActionData action = Model->GetAction(UUDDealActionParticipantInvite::ActionTypeId, { Content.DealId, Content.FactionId } );
	Model->RequestAction(Model->GetDecisionAction(Content.FactionId, EUDDecisionType::Offer, UUDDealActionParticipantInviteConsequence::ActionTypeId, action));
}

void UUDInviteItemViewModel::SetContent(FUDDealFactionInfo content)
{
	Content = content;
}

void UUDInviteItemViewModel::SetFactionNameText(FText newFactionNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionNameText, newFactionNameText);
}

FText UUDInviteItemViewModel::GetFactionNameText() const
{
	return FactionNameText;
}

void UUDInviteItemViewModel::SetInviteText(FText newInviteText)
{
	UE_MVVM_SET_PROPERTY_VALUE(InviteText, newInviteText);
}

FText UUDInviteItemViewModel::GetInviteText() const
{
	return InviteText;
}

void UUDInviteItemViewModel::SetBlockedText(FText newBlockedText)
{
	UE_MVVM_SET_PROPERTY_VALUE(BlockedText, newBlockedText);
}

FText UUDInviteItemViewModel::GetBlockedText() const
{
	return BlockedText;
}

void UUDInviteItemViewModel::SetIsInviteableValue(bool newIsInviteableValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsInviteableValue, newIsInviteableValue);
}

bool UUDInviteItemViewModel::GetIsInviteableValue() const
{
	return IsInviteableValue;
}