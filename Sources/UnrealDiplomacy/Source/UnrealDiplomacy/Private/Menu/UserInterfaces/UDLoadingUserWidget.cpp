// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDLoadingUserWidget.h"
#include "Menu/UserInterfaces/UDLoadingViewModel.h"

void UUDLoadingUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDLoadingViewModel* loadingViewModel = CastChecked<UUDLoadingViewModel>(viewModel.Get());
	ViewModel = loadingViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}