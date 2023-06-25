// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDInviteItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"

#define LOCTEXT_NAMESPACE "InviteItem"

void UUDInviteItemViewModel::Initialize()
{
	FText factionName = FText(LOCTEXT("ParticipantItem", "Faction"));
	SetFactionNameText(factionName);
	FText invite = FText(LOCTEXT("ParticipantItem", "Invite"));
	SetInviteText(invite);
}

void UUDInviteItemViewModel::Update()
{
	SetFactionNameText(FText::FromString(Content.FactionName));
}

#undef LOCTEXT_NAMESPACE

void UUDInviteItemViewModel::Invite()
{
	UE_LOG(LogTemp, Log, TEXT("UUDInviteItemViewModel: Invite."));
	Model->RequestAction(Model->GetAction(UUDDealActionParticipantInvite::ActionTypeId, { Content.DealId, Content.FactionId }));
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