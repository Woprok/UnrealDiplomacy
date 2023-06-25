// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDChatUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDChatViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDChatUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDChatViewModel* actionItemViewModel = CastChecked<UUDChatViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDChatUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDChatUserWidget::BindWidgets()
{
}