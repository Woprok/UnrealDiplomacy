// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDDealItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealItemViewModel* dealItemViewModel = CastChecked<UUDDealItemViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealItemUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->GeneralSourceUpdatedEvent.AddUObject(this, &UUDDealItemUserWidget::SetGeneralTabSourceInstance);
	ViewModel->EditationSourceUpdatedEvent.AddUObject(this, &UUDDealItemUserWidget::SetEditationTabSourceInstance);
	ViewModel->ExecutionSourceUpdatedEvent.AddUObject(this, &UUDDealItemUserWidget::SetExecutionTabSourceInstance);
	// Bind viewmodel to widgets.
	GeneralButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealItemViewModel::General);
	EditationButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealItemViewModel::Editation);
	ExecutionButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealItemViewModel::Execution);
}

void UUDDealItemUserWidget::BindWidgets()
{
	DealNameTextWidget = GetWidget<UTextBlock>(TEXT("DealNameText"));
	GeneralTextWidget = GetWidget<UTextBlock>(TEXT("GeneralText"));
	EditationTextWidget = GetWidget<UTextBlock>(TEXT("EditationText"));
	ExecutableTextWidget = GetWidget<UTextBlock>(TEXT("ExecutableText"));

	GeneralButtonWidget = GetWidget<UButton>(TEXT("GeneralButton"));
	EditationButtonWidget = GetWidget<UButton>(TEXT("EditationButton"));
	ExecutionButtonWidget = GetWidget<UButton>(TEXT("ExecutionButton"));

	DealGeneralTabViewWidget = GetWidget<UUDDealGeneralTabUserWidget>(TEXT("DealGeneralTabView"));
	DealEditationTabViewWidget = GetWidget<UUDDealEditationTabUserWidget>(TEXT("DealEditationTabView"));
	DealExecutionTabViewWidget = GetWidget<UUDDealExecutionTabUserWidget>(TEXT("DealExecutionTabView"));
}

void UUDDealItemUserWidget::SetGeneralTabSourceInstance(const TObjectPtr<UUDDealGeneralTabViewModel>& tabViewModel)
{
	DealGeneralTabViewWidget->SetViewModel(tabViewModel);
}

void UUDDealItemUserWidget::SetEditationTabSourceInstance(const TObjectPtr<UUDDealEditationTabViewModel>& tabViewModel)
{
	DealEditationTabViewWidget->SetViewModel(tabViewModel);
}

void UUDDealItemUserWidget::SetExecutionTabSourceInstance(const TObjectPtr<UUDDealExecutionTabViewModel>& tabViewModel)
{
	DealExecutionTabViewWidget->SetViewModel(tabViewModel);
}