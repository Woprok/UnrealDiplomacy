// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDChatViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDChatItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionMessageSend.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "Chat"

void UUDChatViewModel::Setup()
{
	ChatItemViewModelType = UUDChatItemViewModel::StaticClass();

	FText chat = FText(LOCTEXT("Chat", "Chat"));
	SetChatTitleText(chat);
	FText send = FText(LOCTEXT("Chat", "Send"));
	SetSendText(send);
}

void UUDChatViewModel::Refresh()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
	if (!IsTextEditing)
	{
		FString emptyText = TEXT("");
		SetSelectedText(FText::FromString(emptyText));
	}
	UpdateChatItemList();
}

#undef LOCTEXT_NAMESPACE

void UUDChatViewModel::Send()
{
	UE_LOG(LogTemp, Log, TEXT("UUDChatViewModel: Send."));
	FString message = GetSelectedText().ToString();
	Model->RequestAction(Model->GetAction(UUDDealActionMessageSend::ActionTypeId, { Content.DealId }, message));
}

void UUDChatViewModel::SetContent(FUDDealMinimalInfo content)
{
	Content = content;
}

void UUDChatViewModel::UpdateChatItemList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDChatViewModel: UpdateChatItemList."));
	// Retrieve factions
	TArray<FUDChatMessageInfo> messages = Model->GetDealChatHistory(Content.DealId);
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(ChatItemViewModelCollectionName, 
		ChatItemViewModelType, messages.Num());
	// Get rid of all models
	ChatItemViewModelCollection.Empty();
	for (int32 i = 0; i < messages.Num(); i++)
	{
		TObjectPtr<UUDChatItemViewModel> newViewModel = Cast<UUDChatItemViewModel>(viewModels[i]);
		newViewModel->SetContent(messages[i]);
		newViewModel->Refresh();
		ChatItemViewModelCollection.Add(newViewModel);
	}

	SetChatItemList(FUDViewModelList(viewModels));
}

void UUDChatViewModel::StartTextEditation(const FText& InText)
{
	IsTextEditing = true;
}

void UUDChatViewModel::StopTextEditation(const FText& InText, ETextCommit::Type CommitMethod)
{
	IsTextEditing = false;
	// Update only if value was changed.
	FText newText = InText;
	FText oldText = GetSelectedText();
	// TODO find source of the bug that causes the binding for EditableTextBox to not update automatically.
	if (!oldText.EqualTo(newText))
	{
		SetSelectedText(newText);
	}
}

void UUDChatViewModel::SetChatTitleText(FText newChatTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ChatTitleText, newChatTitleText);
}

FText UUDChatViewModel::GetChatTitleText() const
{
	return ChatTitleText;
}

void UUDChatViewModel::SetSendText(FText newSendText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SendText, newSendText);
}

FText UUDChatViewModel::GetSendText() const
{
	return SendText;
}

void UUDChatViewModel::SetSelectedText(FText newSelectedText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SelectedText, newSelectedText);
}

FText UUDChatViewModel::GetSelectedText() const
{
	return SelectedText;
}

void UUDChatViewModel::SetChatItemList(FUDViewModelList newChatItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(ChatItemList, newChatItemList);
}

FUDViewModelList UUDChatViewModel::GetChatItemList() const
{
	return ChatItemList;
}