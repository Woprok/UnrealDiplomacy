// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemViewModel.h"
#include "Components/Button.h"

void UUDTertiaryPointItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDTertiaryPointItemViewModel* itemViewModel = Cast<UUDTertiaryPointItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDTertiaryPointItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTertiaryPointItemViewModel* actionItemViewModel = CastChecked<UUDTertiaryPointItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTertiaryPointItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CreateTertiaryPointButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTertiaryPointItemViewModel::CreateTertiaryPoint);
}

void UUDTertiaryPointItemUserWidget::BindWidgets()
{
	CreateTertiaryPointButtonWidget = GetWidget<UButton>(TEXT("CreateTertiaryPointButton"));
}