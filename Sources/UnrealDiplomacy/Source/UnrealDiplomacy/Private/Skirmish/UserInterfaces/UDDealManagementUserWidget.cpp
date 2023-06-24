// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDDealManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDDealManagementViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDDealItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDDealManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealManagementViewModel* dealManagementViewModel = CastChecked<UUDDealManagementViewModel>(viewModel.Get());
	ViewModel = dealManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealManagementUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->ActiveDealItemChangedEvent.AddUObject(this, &UUDDealManagementUserWidget::SetActiveItemInstance);
	ViewModel->HistoryDealItemChangedEvent.AddUObject(this, &UUDDealManagementUserWidget::SetHistoryItemInstance);
	// Bind viewmodel to widgets.
	CreateButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealManagementViewModel::CreateDeal);
	ActiveButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealManagementViewModel::SwitchActiveDeals);
	HistoryButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealManagementViewModel::SwitchHistoryDeals);
	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealManagementViewModel::Close);
	FirstButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealManagementViewModel::First);
	PreviousButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealManagementViewModel::Previous);
	NextButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealManagementViewModel::Next);
	LastButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealManagementViewModel::Last);
}

void UUDDealManagementUserWidget::BindWidgets()
{
	DealManagementTitleTextWidget = GetWidget<UTextBlock>(TEXT("DealManagementTitleText"));
	DealCountTextWidget = GetWidget<UTextBlock>(TEXT("DealCountText"));
	CloseTextWidget = GetWidget<UTextBlock>(TEXT("CloseText"));
	FirstTextWidget = GetWidget<UTextBlock>(TEXT("FirstText"));
	PreviousTextWidget = GetWidget<UTextBlock>(TEXT("PreviousText"));
	NextTextWidget = GetWidget<UTextBlock>(TEXT("NextText"));
	LastTextWidget = GetWidget<UTextBlock>(TEXT("LastText"));
	CreateTextWidget = GetWidget<UTextBlock>(TEXT("CreateText"));
	LastTextWidget = GetWidget<UTextBlock>(TEXT("ActiveText"));
	LastTextWidget = GetWidget<UTextBlock>(TEXT("HistoryText"));

	CreateButtonWidget = GetWidget<UButton>(TEXT("CreateButton"));
	ActiveButtonWidget = GetWidget<UButton>(TEXT("ActiveButton"));
	HistoryButtonWidget = GetWidget<UButton>(TEXT("HistoryButton"));

	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
	FirstButtonWidget = GetWidget<UButton>(TEXT("FirstButton"));
	PreviousButtonWidget = GetWidget<UButton>(TEXT("PreviousButton"));
	NextButtonWidget = GetWidget<UButton>(TEXT("NextButton"));
	LastButtonWidget = GetWidget<UButton>(TEXT("LastButton"));

	ActiveDealItemInstanceWidget = GetWidget<UUDDealItemUserWidget>(TEXT("ActiveDealItemInstance"));
	HistoryDealItemInstanceWidget = GetWidget<UUDDealItemUserWidget>(TEXT("HistoryDealItemInstance"));
}

void UUDDealManagementUserWidget::SetActiveItemInstance(const TObjectPtr<UUDDealItemViewModel>& dealItemViewModel)
{
	ActiveDealItemInstanceWidget->SetViewModel(dealItemViewModel);
}

void UUDDealManagementUserWidget::SetHistoryItemInstance(const TObjectPtr<UUDDealItemViewModel>& dealItemViewModel)
{
	HistoryDealItemInstanceWidget->SetViewModel(dealItemViewModel);
}