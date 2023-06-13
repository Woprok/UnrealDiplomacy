// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDResourcePanelUserWidget.h"
#include "Skirmish/UserInterfaces/UDResourcePanelViewModel.h"
#include "Skirmish/UserInterfaces/UDResourceItemViewModel.h"
#include "Components/ListView.h"

void UUDResourcePanelUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDResourcePanelViewModel* resourcePanelViewModel = CastChecked<UUDResourcePanelViewModel>(viewModel.Get());
	ViewModel = resourcePanelViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDResourcePanelUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->ResourceSourceUpdatedEvent.AddUObject(this, &UUDResourcePanelUserWidget::SetFactionSourceCollection);
	// Bind viewmodel to widgets.
}

void UUDResourcePanelUserWidget::BindWidgets()
{
	ResourceItemListWidget = GetWidget<UListView>(TEXT("ResourceItemList"));
}

void UUDResourcePanelUserWidget::SetFactionSourceCollection(const TArray<TObjectPtr<UUDResourceItemViewModel>>& resourceItemViewModels)
{
	ResourceItemListWidget->SetListItems(resourceItemViewModels);
}