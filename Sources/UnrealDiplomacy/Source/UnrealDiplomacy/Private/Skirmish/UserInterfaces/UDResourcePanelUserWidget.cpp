// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDResourcePanelUserWidget.h"
#include "Skirmish/UserInterfaces/UDResourcePanelViewModel.h"

void UUDResourcePanelUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDResourcePanelViewModel* resourcePanelViewModel = CastChecked<UUDResourcePanelViewModel>(viewModel.Get());
	ViewModel = resourcePanelViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}