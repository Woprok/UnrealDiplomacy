// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDTileParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDTileParameterViewModel.h"

void UUDTileParameterUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDTileParameterViewModel* itemViewModel = Cast<UUDTileParameterViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDTileParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTileParameterViewModel* parameterViewModel = CastChecked<UUDTileParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTileParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDTileParameterUserWidget::BindWidgets()
{
}