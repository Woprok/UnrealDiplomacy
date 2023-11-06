// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDPolicySelectorUserWidget.h"
#include "Skirmish/UserInterfaces/UDPolicySelectorViewModel.h"
#include "Skirmish/UserInterfaces/UDPolicySelectItemViewModel.h"
#include "Components/TextBlock.h"
#include "Core/UserInterfaces/Components/UDListView.h"

void UUDPolicySelectorUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPolicySelectorViewModel* policySelectorViewModel = CastChecked<UUDPolicySelectorViewModel>(viewModel.Get());
	ViewModel = policySelectorViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDPolicySelectorUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
}

void UUDPolicySelectorUserWidget::BindWidgets()
{
	PolicyTitleTextWidget = GetWidget<UTextBlock>(TEXT("PolicyTitleText"));
	PolicyItemListWidget = GetWidget<UUDListView>(TEXT("PolicyItemList"));
}