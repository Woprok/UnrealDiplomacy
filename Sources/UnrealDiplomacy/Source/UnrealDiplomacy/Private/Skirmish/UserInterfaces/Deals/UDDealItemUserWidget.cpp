// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealItemViewModel.h"
#include "Components/Button.h"

void UUDDealItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealItemViewModel* dealItemViewModel = CastChecked<UUDDealItemViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	GeneralButtonWidget->OnClicked.Clear();
	EditationButtonWidget->OnClicked.Clear();
	ExecutionButtonWidget->OnClicked.Clear();

	GeneralButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealItemViewModel::General);
	EditationButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealItemViewModel::Editation);
	ExecutionButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealItemViewModel::Execution);
}

void UUDDealItemUserWidget::BindWidgets()
{
	GeneralButtonWidget = GetWidget<UButton>(TEXT("GeneralButton"));
	EditationButtonWidget = GetWidget<UButton>(TEXT("EditationButton"));
	ExecutionButtonWidget = GetWidget<UButton>(TEXT("ExecutionButton"));
}