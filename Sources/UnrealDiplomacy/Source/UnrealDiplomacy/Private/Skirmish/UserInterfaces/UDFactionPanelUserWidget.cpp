// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionPanelUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionPanelViewModel.h"
#include "Components/TextBlock.h"
#include "Core/UserInterfaces/Components/UDListView.h"

void UUDFactionPanelUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionPanelViewModel* factionPanelViewModel = CastChecked<UUDFactionPanelViewModel>(viewModel.Get());
	ViewModel = factionPanelViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionPanelUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
}

void UUDFactionPanelUserWidget::BindWidgets()
{
	NameHeaderTextWidget = GetWidget<UTextBlock>(TEXT("NameHeaderText"));
	ControllerHeaderTextWidget = GetWidget<UTextBlock>(TEXT("ControllerHeaderText"));
	FactionItemListWidget = GetWidget<UUDListView>(TEXT("FactionItemList"));
}