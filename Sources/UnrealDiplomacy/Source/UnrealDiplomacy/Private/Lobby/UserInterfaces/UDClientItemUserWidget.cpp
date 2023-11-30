// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDClientItemUserWidget.h"
#include "Lobby/UserInterfaces/UDClientItemViewModel.h"

void UUDClientItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDClientItemViewModel* clientItemViewModel = CastChecked<UUDClientItemViewModel>(viewModel.Get());
	ViewModel = clientItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}