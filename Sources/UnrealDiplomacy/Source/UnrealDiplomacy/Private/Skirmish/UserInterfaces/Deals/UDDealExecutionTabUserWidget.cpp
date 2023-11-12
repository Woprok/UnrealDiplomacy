// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabViewModel.h"

void UUDDealExecutionTabUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealExecutionTabViewModel* dealItemViewModel = CastChecked<UUDDealExecutionTabViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}