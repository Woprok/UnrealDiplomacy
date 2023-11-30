// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDParticipantItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDParticipantItemViewModel.h"

void UUDParticipantItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDParticipantItemViewModel* actionItemViewModel = CastChecked<UUDParticipantItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}