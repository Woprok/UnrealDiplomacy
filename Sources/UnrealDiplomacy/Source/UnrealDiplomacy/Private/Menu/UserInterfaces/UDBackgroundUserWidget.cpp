// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDBackgroundUserWidget.h"
#include "Menu/UserInterfaces/UDBackgroundViewModel.h"

void UUDBackgroundUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDBackgroundViewModel* backgroundViewModel = CastChecked<UUDBackgroundViewModel>(viewModel.Get());
	ViewModel = backgroundViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}