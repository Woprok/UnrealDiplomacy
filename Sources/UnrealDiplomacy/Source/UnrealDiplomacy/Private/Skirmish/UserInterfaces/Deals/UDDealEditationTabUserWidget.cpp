// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDPrimaryPointItemViewModel.h"
#include "Core/UserInterfaces/Components/UDListView.h"

void UUDDealEditationTabUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealEditationTabViewModel* dealItemViewModel = CastChecked<UUDDealEditationTabViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealEditationTabUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
}

void UUDDealEditationTabUserWidget::BindWidgets()
{
	PointItemListWidget = GetWidget<UUDListView>(TEXT("PointItemList"));
}