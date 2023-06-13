// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDResourceItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDResourceItemViewModel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUDResourceItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDResourceItemViewModel* itemViewModel = Cast<UUDResourceItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDResourceItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDResourceItemViewModel* resourceItemViewModel = CastChecked<UUDResourceItemViewModel>(viewModel.Get());
	ViewModel = resourceItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDResourceItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDResourceItemUserWidget::BindWidgets()
{
	AmountTextWidget = GetWidget<UTextBlock>(TEXT("AmountText"));
	IconImageWidget = GetWidget<UImage>(TEXT("IconImage"));
}