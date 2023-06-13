// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionPanelUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionPanelViewModel.h"
#include "Skirmish/UserInterfaces/UDFactionItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"

void UUDFactionPanelUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionPanelViewModel* factionPanelViewModel = CastChecked<UUDFactionPanelViewModel>(viewModel.Get());
	ViewModel = factionPanelViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionPanelUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->FactionSourceUpdatedEvent.AddUObject(this, &UUDFactionPanelUserWidget::SetFactionSourceCollection);
	// Bind viewmodel to widgets.
}

void UUDFactionPanelUserWidget::BindWidgets()
{
	NameHeaderTextWidget = GetWidget<UTextBlock>(TEXT("NameHeaderText"));
	ControllerHeaderTextWidget = GetWidget<UTextBlock>(TEXT("ControllerHeaderText"));
	FactionItemListWidget = GetWidget<UListView>(TEXT("FactionItemList"));
}

void UUDFactionPanelUserWidget::SetFactionSourceCollection(const TArray<TObjectPtr<UUDFactionItemViewModel>>& factionItemViewModels)
{
	FactionItemListWidget->SetListItems(factionItemViewModels);
}