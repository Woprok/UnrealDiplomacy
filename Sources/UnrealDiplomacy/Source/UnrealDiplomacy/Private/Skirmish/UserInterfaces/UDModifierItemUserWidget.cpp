// Copyright Miroslav Valach


#include "Skirmish/UserInterfaces/UDModifierItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDModifierItemViewModel.h"
#include "Components/Image.h"

void UUDModifierItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDModifierItemViewModel* itemViewModel = Cast<UUDModifierItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDModifierItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDModifierItemViewModel* resourceItemViewModel = CastChecked<UUDModifierItemViewModel>(viewModel.Get());
	ViewModel = resourceItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}