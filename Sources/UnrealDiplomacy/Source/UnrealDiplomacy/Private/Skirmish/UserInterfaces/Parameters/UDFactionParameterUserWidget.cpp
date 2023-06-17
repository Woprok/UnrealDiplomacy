// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterViewModel.h"

void UUDFactionParameterUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDFactionParameterViewModel* itemViewModel = Cast<UUDFactionParameterViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDFactionParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionParameterViewModel* parameterViewModel = CastChecked<UUDFactionParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDFactionParameterUserWidget::BindWidgets()
{
}