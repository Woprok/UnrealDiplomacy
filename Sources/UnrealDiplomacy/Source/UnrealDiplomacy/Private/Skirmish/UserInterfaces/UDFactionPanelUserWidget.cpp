// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionPanelUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionPanelViewModel.h"

void UUDFactionPanelUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionPanelViewModel* factionPanelViewModel = CastChecked<UUDFactionPanelViewModel>(viewModel.Get());
	ViewModel = factionPanelViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}