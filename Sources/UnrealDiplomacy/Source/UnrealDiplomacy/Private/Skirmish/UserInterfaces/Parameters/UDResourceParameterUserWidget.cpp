// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterViewModel.h"

void UUDResourceParameterUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDResourceParameterViewModel* itemViewModel = Cast<UUDResourceParameterViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDResourceParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDResourceParameterViewModel* parameterViewModel = CastChecked<UUDResourceParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDResourceParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDResourceParameterUserWidget::BindWidgets()
{
}