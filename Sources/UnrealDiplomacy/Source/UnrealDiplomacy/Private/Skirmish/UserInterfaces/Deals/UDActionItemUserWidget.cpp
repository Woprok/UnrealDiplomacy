// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDActionItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDActionItemViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDActionItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDActionItemViewModel* itemViewModel = Cast<UUDActionItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDActionItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDActionItemViewModel* actionItemViewModel = CastChecked<UUDActionItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDActionItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDActionItemUserWidget::BindWidgets()
{
}