// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyMemberViewModel.h"

#define LOCTEXT_NAMESPACE "LobbyMember"

void UUDLobbyMemberViewModel::Initialize()
{
	FText memberSettingsTitle = FText(LOCTEXT("LobbyMember", "Game Settings"));
	SetMemberSettingsTitleText(memberSettingsTitle);
	FText nationName = FText(LOCTEXT("LobbyMember", "Nation Name"));
	SetNationNameText(nationName);
	FText strategy = FText(LOCTEXT("LobbyMember", "Stratagems"));
	SetStrategyText(strategy);
	FText nationNameEditable = FText(LOCTEXT("LobbyMember", "Generic Nation Name"));
	SetNationNameEditableText(nationNameEditable);
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

void UUDLobbyMemberViewModel::SetNationNameText(FText newNationNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NationNameText, newNationNameText);
}

FText UUDLobbyMemberViewModel::GetNationNameText() const
{
	return NationNameText;
}

void UUDLobbyMemberViewModel::SetStrategyText(FText newStrategyText)
{
	UE_MVVM_SET_PROPERTY_VALUE(StrategyText, newStrategyText);
}

FText UUDLobbyMemberViewModel::GetStrategyText() const
{
	return StrategyText;
}

void UUDLobbyMemberViewModel::SetNationNameEditableText(FText newNationNameEditableText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NationNameEditableText, newNationNameEditableText);
	ApplyNationNameChange();
}

FText UUDLobbyMemberViewModel::GetNationNameEditableText() const
{
	return NationNameEditableText;
}