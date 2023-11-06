// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabViewModel.h"
#include "Core/UserInterfaces/Components/UDListView.h"

void UUDDealExecutionTabUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealExecutionTabViewModel* dealItemViewModel = CastChecked<UUDDealExecutionTabViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealExecutionTabUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
}

void UUDDealExecutionTabUserWidget::BindWidgets()
{
	ActionItemListWidget = GetWidget<UUDListView>(TEXT("ActionItemList"));
}