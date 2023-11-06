// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDServerItemUserWidget.h"
#include "Menu/UserInterfaces/UDServerItemViewModel.h"
#include "Components/Button.h"

void UUDServerItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDServerItemViewModel* itemViewModel = Cast<UUDServerItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDServerItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDServerItemViewModel* serverItemViewModel = CastChecked<UUDServerItemViewModel>(viewModel.Get());
	ViewModel = serverItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDServerItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	ItemButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDServerItemViewModel::Join);
}

void UUDServerItemUserWidget::BindWidgets()
{
	ItemButtonWidget = GetWidget<UButton>(TEXT("ItemButton"));
}