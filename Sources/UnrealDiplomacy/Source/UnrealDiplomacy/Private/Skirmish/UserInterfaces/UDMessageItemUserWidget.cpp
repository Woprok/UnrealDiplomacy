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
	OkButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageItemViewModel::Ok);
	RejectButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageItemViewModel::Reject);
}

void UUDMessageItemUserWidget::BindWidgets()
{
	MessageTitleTextWidget = GetWidget<UTextBlock>(TEXT("MessageTitleText"));
	MessageTypeTextWidget = GetWidget<UTextBlock>(TEXT("MessageTypeText"));

	MessageConfirmTextWidget = GetWidget<UTextBlock>(TEXT("MessageConfirmText"));
	MessageRejectTextWidget = GetWidget<UTextBlock>(TEXT("MessageRejectText"));
	OnConfirmTextWidget = GetWidget<UTextBlock>(TEXT("OnConfirmText"));
	OnRejectTextWidget = GetWidget<UTextBlock>(TEXT("OnRejectText"));

	AcceptTextWidget = GetWidget<UTextBlock>(TEXT("AcceptText"));
	OkTextWidget = GetWidget<UTextBlock>(TEXT("OkText"));
	RejectTextWidget = GetWidget<UTextBlock>(TEXT("RejectText"));
	AcceptButtonWidget = GetWidget<UButton>(TEXT("AcceptButton"));
	OkButtonWidget = GetWidget<UButton>(TEXT("OkButton"));
	RejectButtonWidget = GetWidget<UButton>(TEXT("RejectButton"));
}