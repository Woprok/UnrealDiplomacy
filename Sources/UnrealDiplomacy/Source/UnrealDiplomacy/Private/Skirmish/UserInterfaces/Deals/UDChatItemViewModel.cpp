// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDChatItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"

#define LOCTEXT_NAMESPACE "ChatItem"

void UUDChatItemViewModel::Setup()
{
	FText faction = FText(LOCTEXT("ChatItem", "Faction"));
	SetFactionText(faction);
	FText message = FText(LOCTEXT("ChatItem", "No message was send."));
	SetMessageText(message);
}

void UUDChatItemViewModel::Refresh()
{
	SetFactionText(FText::FromString(Content.FactionName));
	SetMessageText(FText::FromString(Content.Message));
}

#undef LOCTEXT_NAMESPACE

void UUDChatItemViewModel::SetContent(FUDChatMessageInfo content)
{
	Content = content;
}

void UUDChatItemViewModel::SetFactionText(FText newFactionText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionText, newFactionText);
}

FText UUDChatItemViewModel::GetFactionText() const
{
	return FactionText;
}

void UUDChatItemViewModel::SetMessageText(FText newMessageText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageText, newMessageText);
}

FText UUDChatItemViewModel::GetMessageText() const
{
	return MessageText;
}