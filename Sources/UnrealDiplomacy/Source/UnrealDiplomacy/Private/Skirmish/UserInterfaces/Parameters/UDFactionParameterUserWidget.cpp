// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDFactionParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionParameterViewModel* parameterViewModel = CastChecked<UUDFactionParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	PreviousButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDFactionParameterViewModel::PreviousFaction);
	NextButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDFactionParameterViewModel::NextFaction);
}

void UUDFactionParameterUserWidget::BindWidgets()
{
	FactionTitleTextWidget = GetWidget<UTextBlock>(TEXT("FactionTitleText"));
	PreviousTextWidget = GetWidget<UTextBlock>(TEXT("PreviousText"));
	NextTextWidget = GetWidget<UTextBlock>(TEXT("NextText"));
	NameTextWidget = GetWidget<UTextBlock>(TEXT("NameText"));
	PreviousButtonWidget = GetWidget<UButton>(TEXT("PreviousButton"));
	NextButtonWidget = GetWidget<UButton>(TEXT("NextButton"));
}