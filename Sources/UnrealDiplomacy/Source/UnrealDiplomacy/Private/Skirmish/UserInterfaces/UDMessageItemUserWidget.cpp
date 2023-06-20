// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDMessageItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDMessageItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDMessageItemViewModel* messageManagementViewModel = CastChecked<UUDMessageItemViewModel>(viewModel.Get());
	ViewModel = messageManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDMessageItemUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
	AcceptButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageItemViewModel::Accept);
	RejectButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageItemViewModel::Reject);
}

void UUDMessageItemUserWidget::BindWidgets()
{
	MessageTitleTextWidget = GetWidget<UTextBlock>(TEXT("MessageTitleText"));
	MessageTextWidget = GetWidget<UTextBlock>(TEXT("MessageText"));
	AcceptTextWidget = GetWidget<UTextBlock>(TEXT("AcceptText"));
	RejectTextWidget = GetWidget<UTextBlock>(TEXT("RejectText"));
	AcceptButtonWidget = GetWidget<UButton>(TEXT("AcceptButton"));
	RejectButtonWidget = GetWidget<UButton>(TEXT("RejectButton"));
}