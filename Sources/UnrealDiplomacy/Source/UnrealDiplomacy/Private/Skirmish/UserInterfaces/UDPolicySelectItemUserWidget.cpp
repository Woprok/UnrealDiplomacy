// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDPolicySelectItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDPolicySelectItemViewModel.h"
#include "Components/Button.h"

void UUDPolicySelectItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDPolicySelectItemViewModel* itemViewModel = Cast<UUDPolicySelectItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

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