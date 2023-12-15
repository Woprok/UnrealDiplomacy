// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDServerItemUserWidget.h"
#include "Menu/UserInterfaces/UDServerItemViewModel.h"
#include "Components/Button.h"

void UUDServerItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDServerItemViewModel* serverItemViewModel = CastChecked<UUDServerItemViewModel>(viewModel.Get());
	ViewModel = serverItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDServerItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	ItemButtonWidget->OnClicked.Clear();

	ItemButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDServerItemViewModel::Join);
}

void UUDServerItemUserWidget::BindWidgets()
{
	ItemButtonWidget = GetWidget<UButton>(TEXT("ItemButton"));
}