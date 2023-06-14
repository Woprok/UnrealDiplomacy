// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDImperialThroneUserWidget.h"
#include "Skirmish/UserInterfaces/UDImperialThroneViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDImperialThroneUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDImperialThroneViewModel* turnViewModel = CastChecked<UUDImperialThroneViewModel>(viewModel.Get());
	ViewModel = turnViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDImperialThroneUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	ThroneButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDImperialThroneViewModel::ThroneAction);
	ThroneTextButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDImperialThroneViewModel::ThroneAction);
	DealButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDImperialThroneViewModel::OpenDeals);
	MessageButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDImperialThroneViewModel::OpenMessages);
}

void UUDImperialThroneUserWidget::BindWidgets()
{
	DealCountTextWidget = GetWidget<UTextBlock>(TEXT("DealCountText"));
	MessageCountTextWidget = GetWidget<UTextBlock>(TEXT("MessageCountText"));
	ThroneTextWidget = GetWidget<UTextBlock>(TEXT("ThroneText"));
	ThroneButtonWidget = GetWidget<UButton>(TEXT("ThroneButton"));
	ThroneTextButtonWidget = GetWidget<UButton>(TEXT("ThroneTextButton"));
	DealButtonWidget = GetWidget<UButton>(TEXT("DealButton"));
	MessageButtonWidget = GetWidget<UButton>(TEXT("MessageButton"));
}