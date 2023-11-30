// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTurnUserWidget.h"
#include "Skirmish/UserInterfaces/UDTurnViewModel.h"
#include "Components/Button.h"

void UUDTurnUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTurnViewModel* turnViewModel = CastChecked<UUDTurnViewModel>(viewModel.Get());
	ViewModel = turnViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTurnUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	FinishTurnButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTurnViewModel::FinishTurn);
}

void UUDTurnUserWidget::BindWidgets()
{
	FinishTurnButtonWidget = GetWidget<UButton>(TEXT("FinishTurnButton"));
}