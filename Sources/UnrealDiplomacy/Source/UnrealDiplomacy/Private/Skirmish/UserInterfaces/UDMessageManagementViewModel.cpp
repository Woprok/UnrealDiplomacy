// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDMessageManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/UDGlobalData.h"

#define LOCTEXT_NAMESPACE "MessageManagement"

FUDMessageInfo GetInvalidMessage()
{
	FUDMessageInfo info;
	info.DecisionId = UUDGlobalData::InvalidActionId;
	info.Content.ActionTypeId = UUDGlobalData::InvalidActionId;
	info.Content.Name = FText(LOCTEXT("MessageManagement", "No Pending Messages")).ToString();
	info.Content.Content = FText(LOCTEXT("MessageManagement", "")).ToString();
	return info;
}

void UUDMessageManagementViewModel::Initialize()
{
	MessageItemType = UUDMessageItemViewModel::StaticClass();

	FText messageTitle = FText(LOCTEXT("MessageManagement", "Message Management"));
	SetMessageManagementTitleText(messageTitle);
	FText messageCount = FText::Format(LOCTEXT("MessageManagement", "{0}/{1}"), 0, 0);
	SetMessageCountText(messageCount);
	
	FText close = FText(LOCTEXT("MessageManagement", "X"));
	SetCloseText(close);
	FText first = FText(LOCTEXT("MessageManagement", "|<<"));
	SetFirstText(first);
	FText previous = FText(LOCTEXT("MessageManagement", "<"));
	SetPreviousText(previous);
	FText next = FText(LOCTEXT("MessageManagement", ">"));
	SetNextText(next);
	FText last = FText(LOCTEXT("MessageManagement", ">>|"));
	SetLastText(last);

	SelectedIndex = UUDGlobalData::InvalidArrayIndex;
	SelectedMessageItem = GetInvalidMessage();

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDMessageManagementViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDMessageManagementViewModel::Update);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	// Retrieve view model for sub content control
	TObjectPtr<UUDViewModel> messageItemModel = hud->GetViewModelCollection(MessageItemInstanceName, MessageItemType);
	MessageItemInstance = Cast<UUDMessageItemViewModel>(messageItemModel);
	// Announce them to widget for additional binding.
	MessageItemChangedEvent.Broadcast(MessageItemInstance);
	// Call initialize so instance is ready to use, once it receives data in runtime.
	MessageItemInstance->FullUpdate();

	Update();
}

void UUDMessageManagementViewModel::Reload()
{
	Update();
}

void UUDMessageManagementViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
	UpdateMessageItems();
}

void UUDMessageManagementViewModel::UpdateSelectedMessageItem()
{
	FText messageCount = FText::Format(LOCTEXT("MessageManagement", "{0}/{1}"), SelectedIndex + 1, Content.Messages.Num());
	SetMessageCountText(messageCount);
	if (SelectedIndex == UUDGlobalData::InvalidArrayIndex)
	{
		MessageItemInstance->InvalidateContent(SelectedMessageItem);
	}
	else 
	{
		MessageItemInstance->SetContent(SelectedMessageItem);
	}
}

#undef LOCTEXT_NAMESPACE

void UUDMessageManagementViewModel::Close()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMessageManagementViewModel: Close."));
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->HideWidget(hud->MessageManagementWidget);
}

void UUDMessageManagementViewModel::UpdateMessageItems()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMessageManagementViewModel: UpdateMessageItem."));
	Content = Model->GetAllLocalRequests();
	SelectedMessageItem = GetSelectedOrDefault(SelectedMessageItem.DecisionId);
	UpdateSelectedMessageItem();
}

#pragma region Navigation

FUDMessageInfo UUDMessageManagementViewModel::GetSelectedOrDefault(int32 desiredSelectedItem)
{
	FUDMessageInfo selected = GetInvalidMessage();
	SelectedIndex = UUDGlobalData::InvalidArrayIndex;
	// Find
	const auto& found = Content.Messages.FindByPredicate(
		[&desiredSelectedItem](const FUDMessageInfo& item) { return item.DecisionId == desiredSelectedItem; }
	);
	if (found)
	{
		selected = *found;
		SelectedIndex = Content.Messages.Find(selected);
	}
	else if (Content.Messages.Num() > 0)
	{
		selected = Content.Messages[0];
		SelectedIndex = 0;
	}

	return selected;
}

void UUDMessageManagementViewModel::First()
{
	if (Content.Messages.Num() > 0)
	{
		SelectedIndex = 0;
		SelectedMessageItem = Content.Messages[SelectedIndex];
		UpdateSelectedMessageItem();
	}
}

void UUDMessageManagementViewModel::Previous()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedIndex - distance >= 0)
	{
		SelectedIndex -= distance;
		SelectedMessageItem = Content.Messages[SelectedIndex];
		UpdateSelectedMessageItem();
	}
}

void UUDMessageManagementViewModel::Next()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedIndex + distance < Content.Messages.Num())
	{
		SelectedIndex += distance;
		SelectedMessageItem = Content.Messages[SelectedIndex];
		UpdateSelectedMessageItem();
	}
}

void UUDMessageManagementViewModel::Last()
{
	if (Content.Messages.Num() > 0)
	{
		SelectedIndex = Content.Messages.Num() - 1;
		SelectedMessageItem = Content.Messages[SelectedIndex];
		UpdateSelectedMessageItem();
	}
}
#pragma endregion

void UUDMessageManagementViewModel::SetMessageManagementTitleText(FText newMessageManagementTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageManagementTitleText, newMessageManagementTitleText);
}

FText UUDMessageManagementViewModel::GetMessageManagementTitleText() const
{
	return MessageManagementTitleText;
}

void UUDMessageManagementViewModel::SetMessageCountText(FText newMessageCountText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageCountText, newMessageCountText);
}

FText UUDMessageManagementViewModel::GetMessageCountText() const
{
	return MessageCountText;
}

void UUDMessageManagementViewModel::SetCloseText(FText newCloseText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CloseText, newCloseText);
}

FText UUDMessageManagementViewModel::GetCloseText() const
{
	return CloseText;
}

void UUDMessageManagementViewModel::SetFirstText(FText newFirstText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FirstText, newFirstText);
}

FText UUDMessageManagementViewModel::GetFirstText() const
{
	return FirstText;
}

void UUDMessageManagementViewModel::SetPreviousText(FText newPreviousText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PreviousText, newPreviousText);
}

FText UUDMessageManagementViewModel::GetPreviousText() const
{
	return PreviousText;
}

void UUDMessageManagementViewModel::SetNextText(FText newNextText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NextText, newNextText);
}

FText UUDMessageManagementViewModel::GetNextText() const
{
	return NextText;
}

void UUDMessageManagementViewModel::SetLastText(FText newLastText)
{
	UE_MVVM_SET_PROPERTY_VALUE(LastText, newLastText);
}

FText UUDMessageManagementViewModel::GetLastText() const
{
	return LastText;
}