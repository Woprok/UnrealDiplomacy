// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDResourceItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDResourceItemViewModel.h"

void UUDResourceItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDResourceItemViewModel* resourceItemViewModel = CastChecked<UUDResourceItemViewModel>(viewModel.Get());
	ViewModel = resourceItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}