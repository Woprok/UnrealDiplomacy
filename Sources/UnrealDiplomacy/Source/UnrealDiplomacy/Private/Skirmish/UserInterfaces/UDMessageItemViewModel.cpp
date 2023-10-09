// Copyright Miroslav Valach
// TODO consider allowing editation of parameters before accepting and implement counter-offers ?
// Counter-offers idea was scrapped ... conflicts with deals

#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "MessageItem"

void UUDMessageItemViewModel::Initialize()
{
	FText messageTitle = FText(LOCTEXT("MessageItem", "Invalid Message"));
	SetMessageTitleText(messageTitle);
	FText message = FText(LOCTEXT("MessageItem", "This is not valid!"));
	SetMessageText(message);
	FText accept = FText(LOCTEXT("MessageItem", "Accept"));
	SetAcceptText(accept);
	FText ok = FText(LOCTEXT("MessageItem", "Ok"));
	SetOkText(ok);
	FText reject = FText(LOCTEXT("MessageItem", "Reject"));
	SetRejectText(reject);
	SetHasContentValue(false);
}

void UUDMessageItemViewModel::Update()
{
	SetMessageTypeText(FText::FromString(Content.Type));
	SetMessageTitleText(FText::FromString(Content.Content.Name));
	SetMessageText(FText::FromString(Content.Content.Content));
	SetMessageAdditionalText(FText::FromString(Content.AdditionalContent.Content));
	SetHasChoicesValue(Content.HasChoices);
}

#undef LOCTEXT_NAMESPACE

void UUDMessageItemViewModel::InvalidateContent(FUDMessageInfo content)
{
	Content = content;
	SetHasContentValue(false);
	Update();
}

void UUDMessageItemViewModel::SetContent(FUDMessageInfo content)
{
	Content = content;
	SetHasContentValue(true);
	Update();
}

void UUDMessageItemViewModel::Accept()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMessageItemViewModel: Accept."));
	Model->RequestAction(Model->GetConfirmAction(Content.DecisionId));
}

void UUDMessageItemViewModel::Ok()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMessageItemViewModel: Ok."));
	Model->RequestAction(Model->GetConfirmAction(Content.DecisionId));
}

void UUDMessageItemViewModel::Reject()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMessageItemViewModel: Reject."));
	Model->RequestAction(Model->GetDeclineAction(Content.DecisionId));
}

void UUDMessageItemViewModel::SetMessageTitleText(FText newMessageTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageTitleText, newMessageTitleText);
}

FText UUDMessageItemViewModel::GetMessageTitleText() const
{
	return MessageTitleText;
}

void UUDMessageItemViewModel::SetMessageTypeText(FText newMessageTypeText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageTypeText, newMessageTypeText);
}

FText UUDMessageItemViewModel::GetMessageTypeText() const
{
	return MessageTypeText;
}

void UUDMessageItemViewModel::SetMessageText(FText newMessageText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageText, newMessageText);
}

FText UUDMessageItemViewModel::GetMessageText() const
{
	return MessageText;
}

void UUDMessageItemViewModel::SetMessageAdditionalText(FText newMessageAdditionalText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageAdditionalText, newMessageAdditionalText);
}

FText UUDMessageItemViewModel::GetMessageAdditionalText() const
{
	return MessageAdditionalText;
}

void UUDMessageItemViewModel::SetAcceptText(FText newAcceptText)
{
	UE_MVVM_SET_PROPERTY_VALUE(AcceptText, newAcceptText);
}

FText UUDMessageItemViewModel::GetAcceptText() const
{
	return AcceptText;
}

void UUDMessageItemViewModel::SetOkText(FText newOkText)
{
	UE_MVVM_SET_PROPERTY_VALUE(OkText, newOkText);
}

FText UUDMessageItemViewModel::GetOkText() const
{
	return OkText;
}

void UUDMessageItemViewModel::SetRejectText(FText newRejectText)
{
	UE_MVVM_SET_PROPERTY_VALUE(RejectText, newRejectText);
}

FText UUDMessageItemViewModel::GetRejectText() const
{
	return RejectText;
}

void UUDMessageItemViewModel::SetHasContentValue(bool newHasContentValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasContentValue, newHasContentValue);
}

bool UUDMessageItemViewModel::GetHasContentValue() const
{
	return HasContentValue;
}

void UUDMessageItemViewModel::SetHasChoicesValue(bool newHasChoicesValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasChoicesValue, newHasChoicesValue);
}

bool UUDMessageItemViewModel::GetHasChoicesValue() const
{
	return HasChoicesValue;
}