// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDResourceItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDResourceItemViewModel.h"

void UUDResourceItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDResourceItemViewModel* itemViewModel = Cast<UUDResourceItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDResourceItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDResourceItemViewModel* resourceItemViewModel = CastChecked<UUDResourceItemViewModel>(viewModel.Get());
	ViewModel = resourceItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}