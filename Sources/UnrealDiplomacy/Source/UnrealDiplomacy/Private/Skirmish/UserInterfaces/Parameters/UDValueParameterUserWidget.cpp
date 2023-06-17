// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDValueParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDValueParameterViewModel.h"

void UUDValueParameterUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDValueParameterViewModel* itemViewModel = Cast<UUDValueParameterViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDValueParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDValueParameterViewModel* parameterViewModel = CastChecked<UUDValueParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDValueParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDValueParameterUserWidget::BindWidgets()
{
}