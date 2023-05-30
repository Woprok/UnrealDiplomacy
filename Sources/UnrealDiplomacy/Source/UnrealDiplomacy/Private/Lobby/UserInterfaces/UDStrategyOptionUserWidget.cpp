// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDStrategyOptionUserWidget.h"
#include "Lobby/UserInterfaces/UDStrategyOptionViewModel.h"
#include "Components/Image.h"

void UUDStrategyOptionUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDStrategyOptionViewModel* itemViewModel = Cast<UUDStrategyOptionViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDStrategyOptionUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDStrategyOptionViewModel* strategyOptionViewModel = CastChecked<UUDStrategyOptionViewModel>(viewModel.Get());
	ViewModel = strategyOptionViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDStrategyOptionUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDStrategyOptionUserWidget::BindWidgets()
{
	OptionImageWidget = GetWidget<UImage>(TEXT("OptionImage"));
}