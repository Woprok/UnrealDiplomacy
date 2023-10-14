// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDPolicySelectorUserWidget.h"
#include "Skirmish/UserInterfaces/UDPolicySelectorViewModel.h"
#include "Skirmish/UserInterfaces/UDPolicySelectItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"

void UUDPolicySelectorUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPolicySelectorViewModel* policySelectorViewModel = CastChecked<UUDPolicySelectorViewModel>(viewModel.Get());
	ViewModel = policySelectorViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDPolicySelectorUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->PolicySelectItemSourceUpdatedEvent.AddUObject(this, &UUDPolicySelectorUserWidget::SetPolicySelectItemSourceCollection);
	// Bind viewmodel to widgets.
}

void UUDPolicySelectorUserWidget::BindWidgets()
{
	PolicyTitleTextWidget = GetWidget<UTextBlock>(TEXT("PolicyTitleText"));
	PolicyItemListWidget = GetWidget<UListView>(TEXT("PolicyItemList"));
}

void UUDPolicySelectorUserWidget::SetPolicySelectItemSourceCollection(const TArray<TObjectPtr<UUDPolicySelectItemViewModel>>& policySelectItemViewModels)
{
	PolicyItemListWidget->SetListItems(policySelectItemViewModels);
}