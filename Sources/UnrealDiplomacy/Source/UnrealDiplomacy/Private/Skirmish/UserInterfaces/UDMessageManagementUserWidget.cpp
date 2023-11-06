// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDMessageManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDMessageManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDMessageItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Components/Button.h"

void UUDMessageManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDMessageManagementViewModel* messageManagementViewModel = CastChecked<UUDMessageManagementViewModel>(viewModel.Get());
	ViewModel = messageManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDMessageManagementUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->MessageItemChangedEvent.AddUObject(this, &UUDMessageManagementUserWidget::SetMessageItemSourceInstance);
	// Bind viewmodel to widgets.
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
	MessageItemViewWidget = GetWidget<UUDMessageItemUserWidget>(TEXT("MessageItemView"));
}

void UUDMessageManagementUserWidget::SetMessageItemSourceInstance(const TObjectPtr<UUDMessageItemViewModel>& messageItemViewModel)
{
	MessageItemViewWidget->SetViewModel(messageItemViewModel);
}