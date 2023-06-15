// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDMessageManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDMessageManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ListView.h"

void UUDMessageManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDMessageManagementViewModel* messageManagementViewModel = CastChecked<UUDMessageManagementViewModel>(viewModel.Get());
	ViewModel = messageManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDMessageManagementUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::Close);
	FirstButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::First);
	PreviousButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::Previous);
	NextButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::Next);
	LastButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMessageManagementViewModel::Last);
}

void UUDMessageManagementUserWidget::BindWidgets()
{
	MessageManagementTitleTextWidget = GetWidget<UTextBlock>(TEXT("MessageManagementTitleText"));
	CloseTextWidget = GetWidget<UTextBlock>(TEXT("CloseText"));
	FirstTextWidget = GetWidget<UTextBlock>(TEXT("FirstText"));
	PreviousTextWidget = GetWidget<UTextBlock>(TEXT("PreviousText"));
	NextTextWidget = GetWidget<UTextBlock>(TEXT("NextText"));
	LastTextWidget = GetWidget<UTextBlock>(TEXT("LastText"));
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
	FirstButtonWidget = GetWidget<UButton>(TEXT("FirstButton"));
	PreviousButtonWidget = GetWidget<UButton>(TEXT("PreviousButton"));
	NextButtonWidget = GetWidget<UButton>(TEXT("NextButton"));
	LastButtonWidget = GetWidget<UButton>(TEXT("LastButton"));
}