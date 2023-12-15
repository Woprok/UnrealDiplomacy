// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDStrategyOptionUserWidget.h"
#include "Lobby/UserInterfaces/UDStrategyOptionViewModel.h"
#include "Components/Button.h"

void UUDStrategyOptionUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDStrategyOptionViewModel* strategyOptionViewModel = CastChecked<UUDStrategyOptionViewModel>(viewModel.Get());
	ViewModel = strategyOptionViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDStrategyOptionUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	OptionButtonWidget->OnClicked.Clear();
	OptionButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDStrategyOptionViewModel::Selected);
}

void UUDStrategyOptionUserWidget::BindWidgets()
{
	OptionButtonWidget = GetWidget<UButton>(TEXT("OptionButton"));
}