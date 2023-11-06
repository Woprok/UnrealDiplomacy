// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDJoinGameUserWidget.h"
#include "Menu/UserInterfaces/UDJoinGameViewModel.h"
#include "Components/Button.h"

void UUDJoinGameUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDJoinGameViewModel* joinGameViewModel = CastChecked<UUDJoinGameViewModel>(viewModel.Get());
	ViewModel = joinGameViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDJoinGameUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	BackButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDJoinGameViewModel::Back);
	RefreshButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDJoinGameViewModel::Refresh);
}

void UUDJoinGameUserWidget::BindWidgets()
{
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
	RefreshButtonWidget = GetWidget<UButton>(TEXT("RefreshButton"));
}