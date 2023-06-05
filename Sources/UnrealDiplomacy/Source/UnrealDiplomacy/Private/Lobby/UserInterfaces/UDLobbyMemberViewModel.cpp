// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyMemberViewModel.h"

#define LOCTEXT_NAMESPACE "LobbyMember"

void UUDLobbyMemberViewModel::Initialize()
{
	FText memberSettingsTitle = FText(LOCTEXT("LobbyMember", "Game Settings"));
	SetMemberSettingsTitleText(memberSettingsTitle);
	FText factionName = FText(LOCTEXT("LobbyMember", "Faction Name"));
	SetFactionNameText(factionName);
	FText strategy = FText(LOCTEXT("LobbyMember", "Stratagems"));
	SetStrategyText(strategy);
	FText nationNameEditable = FText(LOCTEXT("LobbyMember", "Generic Nation Name"));
	SetFactionNameEditableText(nationNameEditable);
}

void UUDLobbyMemberViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDLobbyMemberViewModel::ApplyNationNameChange()
{

}

void UUDLobbyMemberViewModel::SetMemberSettingsTitleText(FText newMemberSettingsTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MemberSettingsTitleText, newMemberSettingsTitleText);
}

FText UUDLobbyMemberViewModel::GetMemberSettingsTitleText() const
{
	return MemberSettingsTitleText;
}

void UUDLobbyMemberViewModel::SetFactionNameText(FText newFactionNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionNameText, newFactionNameText);
}

FText UUDLobbyMemberViewModel::GetFactionNameText() const
{
	return FactionNameText;
}

void UUDLobbyMemberViewModel::SetStrategyText(FText newStrategyText)
{
	UE_MVVM_SET_PROPERTY_VALUE(StrategyText, newStrategyText);
}

FText UUDLobbyMemberViewModel::GetStrategyText() const
{
	return StrategyText;
}

void UUDLobbyMemberViewModel::SetFactionNameEditableText(FText newFactionNameEditableText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionNameEditableText, newFactionNameEditableText);
	ApplyNationNameChange();
}

FText UUDLobbyMemberViewModel::GetFactionNameEditableText() const
{
	return FactionNameEditableText;
}