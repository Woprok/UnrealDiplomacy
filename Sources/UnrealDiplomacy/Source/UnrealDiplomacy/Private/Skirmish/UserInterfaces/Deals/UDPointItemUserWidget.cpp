// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPointItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPointItemViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDPointItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDPointItemViewModel* itemViewModel = Cast<UUDPointItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDPointItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPointItemViewModel* actionItemViewModel = CastChecked<UUDPointItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDPointItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CreatePointButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDPointItemViewModel::CreatePoint);
}

void UUDPointItemUserWidget::BindWidgets()
{
	CreatePointTextWidget = GetWidget<UTextBlock>(TEXT("CreatePointText"));

	CreatePointButtonWidget = GetWidget<UButton>(TEXT("CreatePointButton"));
}