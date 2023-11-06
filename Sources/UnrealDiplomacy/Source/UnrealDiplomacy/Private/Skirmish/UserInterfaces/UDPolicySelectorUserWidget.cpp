// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDPolicySelectorUserWidget.h"
#include "Skirmish/UserInterfaces/UDPolicySelectorViewModel.h"

void UUDPolicySelectorUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPolicySelectorViewModel* policySelectorViewModel = CastChecked<UUDPolicySelectorViewModel>(viewModel.Get());
	ViewModel = policySelectorViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}