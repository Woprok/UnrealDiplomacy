// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDServerItemViewModel.h"
#include "Core/UDSessionSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#define LOCTEXT_NAMESPACE "ServerItem"

void UUDServerItemViewModel::Initialize()
{
	FText name = FText(LOCTEXT("ServerItem", "Session Name"));
	SetNameText(name);
	int32 ping = -1;
	SetPingValue(ping);
}

void UUDServerItemViewModel::Update()
{
	if (Content == nullptr)
		return;
	FString name;
	auto& nameOption = Content->Session.SessionSettings.Settings[UD_SETTING_SESSIONNAME];
	nameOption.Data.GetValue(name);
	SetNameText(FText::FromString(name));

	auto ping = Content->PingInMs;
	SetPingValue(ping);
	// Always enabled.
	SetIsEnabled(true);
}

#undef LOCTEXT_NAMESPACE

void UUDServerItemViewModel::SetContent(const FOnlineSessionSearchResult& content)
{
	Content = &content;
}

const FOnlineSessionSearchResult& UUDServerItemViewModel::GetContent() const
{
	return *Content;
}

void UUDServerItemViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDServerItemViewModel::GetNameText() const
{
	return NameText;
}

void UUDServerItemViewModel::SetPingValue(int32 newPingValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(PingValue, newPingValue);
}

int32 UUDServerItemViewModel::GetPingValue() const
{
	return PingValue;
}

void UUDServerItemViewModel::SetIsEnabled(bool newIsEnabled)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsEnabled, newIsEnabled);
}

bool UUDServerItemViewModel::GetIsEnabled() const
{
	return IsEnabled;
}