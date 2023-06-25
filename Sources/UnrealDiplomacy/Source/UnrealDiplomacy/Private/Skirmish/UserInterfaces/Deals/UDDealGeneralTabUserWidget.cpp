// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDDealGeneralTabUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealGeneralTabViewModel* dealItemViewModel = CastChecked<UUDDealGeneralTabViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealGeneralTabUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
}

void UUDDealGeneralTabUserWidget::BindWidgets()
{
}