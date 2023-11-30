// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabViewModel.h"

void UUDDealEditationTabUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealEditationTabViewModel* dealItemViewModel = CastChecked<UUDDealEditationTabViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}