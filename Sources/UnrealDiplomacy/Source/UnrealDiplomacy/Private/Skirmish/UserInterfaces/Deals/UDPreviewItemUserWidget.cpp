// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPreviewItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPreviewItemViewModel.h"

void UUDPreviewItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDPreviewItemViewModel* itemViewModel = Cast<UUDPreviewItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDPreviewItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPreviewItemViewModel* actionItemViewModel = CastChecked<UUDPreviewItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}