// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDActionItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDActionItemViewModel.h"
#include "Components/Button.h"

void UUDActionItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDActionItemViewModel* actionItemViewModel = CastChecked<UUDActionItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDActionItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	AcceptButtonWidget->OnClicked.Clear();
	ChangeButtonWidget->OnClicked.Clear();
	DenyButtonWidget->OnClicked.Clear();
	SabotageButtonWidget->OnClicked.Clear();
	AcceptButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionItemViewModel::Accept);
	ChangeButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionItemViewModel::Change);
	DenyButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionItemViewModel::Deny);
	SabotageButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionItemViewModel::Sabotage);
}

void UUDActionItemUserWidget::BindWidgets()
{
	AcceptButtonWidget = GetWidget<UButton>(TEXT("AcceptButton"));
	ChangeButtonWidget = GetWidget<UButton>(TEXT("ChangeButton"));
	DenyButtonWidget = GetWidget<UButton>(TEXT("DenyButton"));
	SabotageButtonWidget = GetWidget<UButton>(TEXT("SabotageButton"));
}