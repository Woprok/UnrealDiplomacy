// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDDealItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealItemViewModel* dealItemViewModel = CastChecked<UUDDealItemViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealItemUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
}

void UUDDealItemUserWidget::BindWidgets()
{
}