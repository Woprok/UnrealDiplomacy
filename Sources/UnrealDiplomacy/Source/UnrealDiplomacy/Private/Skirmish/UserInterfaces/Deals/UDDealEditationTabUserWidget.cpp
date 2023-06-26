// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDPointItemViewModel.h"
#include "Components/ListView.h"

void UUDDealEditationTabUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealEditationTabViewModel* dealItemViewModel = CastChecked<UUDDealEditationTabViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealEditationTabUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->PointItemSourceUpdatedEvent.AddUObject(this, &UUDDealEditationTabUserWidget::SetPointItemSourceCollection);
	// Bind viewmodel to widgets.
}

void UUDDealEditationTabUserWidget::BindWidgets()
{
	PointItemListWidget = GetWidget<UListView>(TEXT("PointItemList"));
}

void UUDDealEditationTabUserWidget::SetPointItemSourceCollection(const TArray<TObjectPtr<UUDPointItemViewModel>>& itemViewModels)
{
	PointItemListWidget->SetListItems(itemViewModels);
}