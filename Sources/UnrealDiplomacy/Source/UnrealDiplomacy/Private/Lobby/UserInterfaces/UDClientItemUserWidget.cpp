// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDClientItemUserWidget.h"
#include "Lobby/UserInterfaces/UDClientItemViewModel.h"

void UUDClientItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDClientItemViewModel* itemViewModel = Cast<UUDClientItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDClientItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDClientItemViewModel* clientItemViewModel = CastChecked<UUDClientItemViewModel>(viewModel.Get());
	ViewModel = clientItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}