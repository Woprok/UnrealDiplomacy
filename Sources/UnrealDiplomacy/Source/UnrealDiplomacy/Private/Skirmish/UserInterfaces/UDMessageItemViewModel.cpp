// Copyright Miroslav Valach
// TODO consider allowing editation of parameters before accepting and implement counter-offers ?

#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"

#define LOCTEXT_NAMESPACE "MessageItem"

void UUDMessageItemViewModel::Initialize()
{
	FText messageTitle = FText(LOCTEXT("MessageItem", "Invalid Message"));
	SetMessageTitleText(messageTitle);
	FText message = FText(LOCTEXT("MessageItem", "This is not valid!"));
	SetMessageText(message);
	FText accept = FText(LOCTEXT("MessageItem", "Accept"));
	SetAcceptText(accept);
	FText reject = FText(LOCTEXT("MessageItem", "Reject"));
	SetRejectText(reject);
	SetHasContentValue(false);
}

void UUDMessageItemViewModel::Update()
{
	SetMessageTitleText(FText::FromString(Content.Name));
	SetMessageText(FText::FromString(Content.Content));
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
	FUDActionData sourceAction = Model->GetPendingRequest(Content.RequestId);
	Model->RequestAction(Model->GetAcceptAction(Content.AcceptId, sourceAction));
}

void UUDMessageItemViewModel::Reject()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMessageItemViewModel: Reject."));
	FUDActionData sourceAction = Model->GetPendingRequest(Content.RequestId);
	Model->RequestAction(Model->GetRejectAction(Content.RejectId, sourceAction));
}

void UUDMessageItemViewModel::SetMessageTitleText(FText newMessageTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageTitleText, newMessageTitleText);
}

FText UUDMessageItemViewModel::GetMessageTitleText() const
{
	return MessageTitleText;
}

void UUDMessageItemViewModel::SetMessageText(FText newMessageText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageText, newMessageText);
}

FText UUDMessageItemViewModel::GetMessageText() const
{
	return MessageText;
}

void UUDMessageItemViewModel::SetAcceptText(FText newAcceptText)
{
	UE_MVVM_SET_PROPERTY_VALUE(AcceptText, newAcceptText);
}

FText UUDMessageItemViewModel::GetAcceptText() const
{
	return AcceptText;
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