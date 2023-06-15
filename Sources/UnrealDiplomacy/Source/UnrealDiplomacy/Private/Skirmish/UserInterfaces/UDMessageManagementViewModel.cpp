// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDMessageManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "MessageManagement"

void UUDMessageManagementViewModel::Initialize()
{
	//MessageItemViewModelType = UUDMessageItemViewModel::StaticClass();

	FText messageTitle = FText(LOCTEXT("MessageManagement", "Message Management"));
	SetMessageManagementTitleText(messageTitle);

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


	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	//hud->OnMessageSelectedEvent.AddUniqueDynamic(this, &UUDMessageManagementViewModel::OnMessageSelected);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDMessageManagementViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDMessageManagementViewModel::Update);

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
	//FUDMessageMinimalInfo faction = Model->GetMessageInfo(SelectedMessageId);
	//SetMessageNameText(FText::FromString(faction.Name));

	UpdateMessageItem();
}

#undef LOCTEXT_NAMESPACE

void UUDMessageManagementViewModel::Close()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMessageManagementViewModel: Close."));
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->HideWidget(hud->MessageManagementWidget);
}

//void UUDMessageManagementViewModel::OnMessageSelected(int32 selectedMessageId)
//{
//	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
//	hud->ShowWidget(hud->MessageManagementWidget);
//
//	if (SelectedMessageId != selectedMessageId)
//	{
//		SelectedMessageId = selectedMessageId;
//		Update();
//	}
//}

void UUDMessageManagementViewModel::UpdateMessageItem()
{
	//UE_LOG(LogTemp, Log, TEXT("UUDMessagePanelViewModel: UpdateMessageList."));
	//// Retrieve factions
	//TArray<FUDMessageInteractionInfo> interactions = Model->GetMessageInteractionList();
	//// Retrieve enough models
	//TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	//TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(MessageInteractionViewModelCollectionName, MessageInteractionViewModelType, interactions.Num());
	//// Get rid of all models
	//MessageInteractionViewModelCollection.Empty();
	//for (int32 i = 0; i < interactions.Num(); i++)
	//{
	//	TObjectPtr<UUDMessageInteractionViewModel> newViewModel = Cast<UUDMessageInteractionViewModel>(viewModels[i]);
	//	newViewModel->SetContent(interactions[i]);
	//	newViewModel->FullUpdate();
	//	MessageInteractionViewModelCollection.Add(newViewModel);
	//}
	//
	//MessageInteractionSourceUpdatedEvent.Broadcast(MessageInteractionViewModelCollection);
}

#pragma region Navigation
void UUDMessageManagementViewModel::First()
{

}

void UUDMessageManagementViewModel::Previous()
{
	//int32 prev = FMath::Max(0, CurrentItemShown - 1);
	//if (prev == CurrentItemShown)
	//{
	//	prev = LastRequestArray.Num() - 1;
	//}
	//UpdateView(LastRequestArray.Num(), LastRequestArray[prev], prev);
}

void UUDMessageManagementViewModel::Next()
{
	//int32 next = FMath::Min(LastRequestArray.Num() - 1, CurrentItemShown + 1);
	//if (next == CurrentItemShown)
	//{
	//	next = 0;
	//}
	//UpdateView(LastRequestArray.Num(), LastRequestArray[next], next);
}

void UUDMessageManagementViewModel::Last()
{

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