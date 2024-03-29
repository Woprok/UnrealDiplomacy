// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDMessageManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDMessageManagementViewModel.h"
#include "Components/Button.h"

void UUDMessageManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDMessageManagementViewModel* messageManagementViewModel = CastChecked<UUDMessageManagementViewModel>(viewModel.Get());
	ViewModel = messageManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDMessageManagementUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CloseButtonWidget->OnClicked.Clear();
	FirstButtonWidget->OnClicked.Clear();
	PreviousButtonWidget->OnClicked.Clear();
	NextButtonWidget->OnClicked.Clear();
	LastButtonWidget->OnClicked.Clear();

	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::Close);
	FirstButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::First);
	PreviousButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::Previous);
	NextButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::Next);
	LastButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::Last);
}

void UUDMessageManagementUserWidget::BindWidgets()
{
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
	FirstButtonWidget = GetWidget<UButton>(TEXT("FirstButton"));
	PreviousButtonWidget = GetWidget<UButton>(TEXT("PreviousButton"));
	NextButtonWidget = GetWidget<UButton>(TEXT("NextButton"));
	LastButtonWidget = GetWidget<UButton>(TEXT("LastButton"));
}