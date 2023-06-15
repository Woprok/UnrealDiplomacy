// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDMessageItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDMessageItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDMessageItemViewModel* messageManagementViewModel = CastChecked<UUDMessageItemViewModel>(viewModel.Get());
	ViewModel = messageManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDMessageItemUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
}

void UUDMessageItemUserWidget::BindWidgets()
{
}