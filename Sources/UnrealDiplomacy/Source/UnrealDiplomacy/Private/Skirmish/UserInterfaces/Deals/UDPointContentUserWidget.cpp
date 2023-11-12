// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPointContentUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Components/Button.h"

void UUDPointContentUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPointContentViewModel* actionItemViewModel = CastChecked<UUDPointContentViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}