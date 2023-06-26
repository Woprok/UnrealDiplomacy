// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDActionItemViewModel.h"
#include "Components/ListView.h"

void UUDDealExecutionTabUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealExecutionTabViewModel* dealItemViewModel = CastChecked<UUDDealExecutionTabViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealExecutionTabUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->ActionItemSourceUpdatedEvent.AddUObject(this, &UUDDealExecutionTabUserWidget::SetActionItemSourceCollection);
	// Bind viewmodel to widgets.
}

void UUDDealExecutionTabUserWidget::BindWidgets()
{
	ActionItemListWidget = GetWidget<UListView>(TEXT("ActionItemList"));
}

void UUDDealExecutionTabUserWidget::SetActionItemSourceCollection(const TArray<TObjectPtr<UUDActionItemViewModel>>& itemViewModels)
{
	ActionItemListWidget->SetListItems(itemViewModels);
}