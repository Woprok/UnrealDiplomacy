// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDPolicySelectItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDPolicySelectItemViewModel.h"
#include "Components/Button.h"

void UUDPolicySelectItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPolicySelectItemViewModel* policyItemViewModel = CastChecked<UUDPolicySelectItemViewModel>(viewModel.Get());
	ViewModel = policyItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDPolicySelectItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	SelectButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDPolicySelectItemViewModel::Select);
}

void UUDPolicySelectItemUserWidget::BindWidgets()
{
	SelectButtonWidget = GetWidget<UButton>(TEXT("SelectButton"));
}