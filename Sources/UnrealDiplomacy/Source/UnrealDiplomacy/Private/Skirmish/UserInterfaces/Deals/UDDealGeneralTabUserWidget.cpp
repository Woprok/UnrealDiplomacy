// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDChatUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDChatViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDParticipantItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDInviteItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ListView.h"

void UUDDealGeneralTabUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealGeneralTabViewModel* dealItemViewModel = CastChecked<UUDDealGeneralTabViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealGeneralTabUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->ParticipantItemSourceUpdatedEvent.AddUObject(this, &UUDDealGeneralTabUserWidget::SetParticipantSourceCollection);
	ViewModel->InviteItemSourceUpdatedEvent.AddUObject(this, &UUDDealGeneralTabUserWidget::SetInviteSourceCollection);
	ViewModel->ChatSourceUpdatedEvent.AddUObject(this, &UUDDealGeneralTabUserWidget::SetChatSourceInstance);
	// Bind viewmodel to widgets.
	ChangeReadyButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::ChangeReady);
	ChangeVoteButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::ChangeVote);
	LeaveButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::Leave);
	CancelButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::Cancel);
}

void UUDDealGeneralTabUserWidget::BindWidgets()
{
	StateTitleTextWidget = GetWidget<UTextBlock>(TEXT("StateTitleText"));
	StateTextWidget = GetWidget<UTextBlock>(TEXT("StateText"));
	ParticipantTextWidget = GetWidget<UTextBlock>(TEXT("ParticipantText"));
	ReadyCountTextWidget = GetWidget<UTextBlock>(TEXT("ReadyCountText"));
	VoteCountTextWidget = GetWidget<UTextBlock>(TEXT("VoteCountText"));
	InviteableTextWidget = GetWidget<UTextBlock>(TEXT("InviteableText"));

	ReadyTextWidget = GetWidget<UTextBlock>(TEXT("ReadyText"));
	VoteTextWidget = GetWidget<UTextBlock>(TEXT("VoteText"));
	LeaveTextWidget = GetWidget<UTextBlock>(TEXT("LeaveText"));
	CancelTextWidget = GetWidget<UTextBlock>(TEXT("CancelText"));

	ChangeReadyButtonWidget = GetWidget<UButton>(TEXT("ChangeReadyButton"));
	ChangeVoteButtonWidget = GetWidget<UButton>(TEXT("ChangeVoteButton"));
	LeaveButtonWidget = GetWidget<UButton>(TEXT("LeaveButton"));
	CancelButtonWidget = GetWidget<UButton>(TEXT("CancelButton"));

	ParticipantItemListWidget = GetWidget<UListView>(TEXT("ParticipantItemList"));
	InviteItemListWidget = GetWidget<UListView>(TEXT("InviteItemList"));
	DealChatViewWidget = GetWidget<UUDChatUserWidget>(TEXT("DealChatView"));
}

void UUDDealGeneralTabUserWidget::SetParticipantSourceCollection(const TArray<TObjectPtr<UUDParticipantItemViewModel>>& itemViewModels)
{
	ParticipantItemListWidget->SetListItems(itemViewModels);
}

void UUDDealGeneralTabUserWidget::SetInviteSourceCollection(const TArray<TObjectPtr<UUDInviteItemViewModel>>& itemViewModels)
{
	InviteItemListWidget->SetListItems(itemViewModels);
}

void UUDDealGeneralTabUserWidget::SetChatSourceInstance(const TObjectPtr<UUDChatViewModel>& chatViewModel)
{
	DealChatViewWidget->SetViewModel(chatViewModel);
}