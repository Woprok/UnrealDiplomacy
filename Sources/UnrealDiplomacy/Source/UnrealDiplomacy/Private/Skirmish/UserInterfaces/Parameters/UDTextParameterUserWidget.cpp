// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDTextParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDTextParameterViewModel.h"

void UUDTextParameterUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDTextParameterViewModel* itemViewModel = Cast<UUDTextParameterViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDTextParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTextParameterViewModel* parameterViewModel = CastChecked<UUDTextParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTextParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDTextParameterUserWidget::BindWidgets()
{
}