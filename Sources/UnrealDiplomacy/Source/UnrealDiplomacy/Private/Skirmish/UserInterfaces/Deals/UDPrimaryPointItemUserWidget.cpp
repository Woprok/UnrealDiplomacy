// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPrimaryPointItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPrimaryPointItemViewModel.h"
#include "Components/Button.h"

void UUDPrimaryPointItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDPrimaryPointItemViewModel* itemViewModel = Cast<UUDPrimaryPointItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDPrimaryPointItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPrimaryPointItemViewModel* actionItemViewModel = CastChecked<UUDPrimaryPointItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDPrimaryPointItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CreatePointButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDPrimaryPointItemViewModel::CreatePoint);
}

void UUDPrimaryPointItemUserWidget::BindWidgets()
{
	CreatePointButtonWidget = GetWidget<UButton>(TEXT("CreatePointButton"));
}