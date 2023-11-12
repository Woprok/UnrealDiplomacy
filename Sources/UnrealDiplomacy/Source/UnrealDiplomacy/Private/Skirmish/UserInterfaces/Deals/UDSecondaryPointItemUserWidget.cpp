// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDSecondaryPointItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDSecondaryPointItemViewModel.h"
#include "Components/Button.h"

void UUDSecondaryPointItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDSecondaryPointItemViewModel* itemViewModel = Cast<UUDSecondaryPointItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDSecondaryPointItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDSecondaryPointItemViewModel* actionItemViewModel = CastChecked<UUDSecondaryPointItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDSecondaryPointItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CreateSecondaryPointButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDSecondaryPointItemViewModel::CreateSecondaryPoint);
}

void UUDSecondaryPointItemUserWidget::BindWidgets()
{
	CreateSecondaryPointButtonWidget = GetWidget<UButton>(TEXT("CreateSecondaryPointButton"));
}