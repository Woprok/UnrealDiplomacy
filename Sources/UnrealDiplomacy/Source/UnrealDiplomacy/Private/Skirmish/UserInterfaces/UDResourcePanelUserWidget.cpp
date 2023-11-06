// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDResourcePanelUserWidget.h"
#include "Skirmish/UserInterfaces/UDResourcePanelViewModel.h"
#include "Core/UserInterfaces/Components/UDListView.h"

void UUDResourcePanelUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDResourcePanelViewModel* resourcePanelViewModel = CastChecked<UUDResourcePanelViewModel>(viewModel.Get());
	ViewModel = resourcePanelViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDResourcePanelUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
}

void UUDResourcePanelUserWidget::BindWidgets()
{
	ResourceItemListWidget = GetWidget<UUDListView>(TEXT("ResourceItemList"));
}