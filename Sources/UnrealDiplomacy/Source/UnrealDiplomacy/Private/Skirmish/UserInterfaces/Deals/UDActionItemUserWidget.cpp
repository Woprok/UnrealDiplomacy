// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDActionItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDActionItemViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDActionItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDActionItemViewModel* itemViewModel = Cast<UUDActionItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDActionItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDActionItemViewModel* actionItemViewModel = CastChecked<UUDActionItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDActionItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	AcceptButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionItemViewModel::Accept);
	ChangeButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionItemViewModel::Change);
	DenyButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionItemViewModel::Deny);
	SabotageButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionItemViewModel::Sabotage);
}

void UUDActionItemUserWidget::BindWidgets()
{
	ActionTextWidget = GetWidget<UTextBlock>(TEXT("ActionText"));
	AcceptTextWidget = GetWidget<UTextBlock>(TEXT("AcceptText"));
	ChangeTextWidget = GetWidget<UTextBlock>(TEXT("ChangeText"));
	DenyTextWidget = GetWidget<UTextBlock>(TEXT("DenyText"));
	SabotageTextWidget = GetWidget<UTextBlock>(TEXT("SabotageText"));

	AcceptButtonWidget = GetWidget<UButton>(TEXT("AcceptButton"));
	ChangeButtonWidget = GetWidget<UButton>(TEXT("ChangeButton"));
	DenyButtonWidget = GetWidget<UButton>(TEXT("DenyButton"));
	SabotageButtonWidget = GetWidget<UButton>(TEXT("SabotageButton"));
}