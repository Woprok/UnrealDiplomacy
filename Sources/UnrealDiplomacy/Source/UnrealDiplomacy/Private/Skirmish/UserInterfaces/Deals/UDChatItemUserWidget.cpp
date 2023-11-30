// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDChatItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDChatItemViewModel.h"
#include "Components/Button.h"

void UUDChatItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDChatItemViewModel* actionItemViewModel = CastChecked<UUDChatItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}