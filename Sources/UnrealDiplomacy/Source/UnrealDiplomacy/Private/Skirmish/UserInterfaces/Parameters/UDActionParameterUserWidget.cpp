// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDActionParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDActionParameterViewModel.h"

void UUDActionParameterUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDActionParameterViewModel* itemViewModel = Cast<UUDActionParameterViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDActionParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDActionParameterViewModel* parameterViewModel = CastChecked<UUDActionParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDActionParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDActionParameterUserWidget::BindWidgets()
{
}