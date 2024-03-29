// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDMessageItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Components/Button.h"

void UUDMessageItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDMessageItemViewModel* messageManagementViewModel = CastChecked<UUDMessageItemViewModel>(viewModel.Get());
	ViewModel = messageManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDMessageItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	AcceptButtonWidget->OnClicked.Clear();
	OkButtonWidget->OnClicked.Clear();
	RejectButtonWidget->OnClicked.Clear();

	AcceptButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageItemViewModel::Accept);
	OkButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageItemViewModel::Ok);
	RejectButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageItemViewModel::Reject);
}

void UUDMessageItemUserWidget::BindWidgets()
{
	AcceptButtonWidget = GetWidget<UButton>(TEXT("AcceptButton"));
	OkButtonWidget = GetWidget<UButton>(TEXT("OkButton"));
	RejectButtonWidget = GetWidget<UButton>(TEXT("RejectButton"));
}