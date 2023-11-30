// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPreviewItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPreviewItemViewModel.h"

void UUDPreviewItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPreviewItemViewModel* actionItemViewModel = CastChecked<UUDPreviewItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}