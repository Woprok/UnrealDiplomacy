// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDActionParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDActionParameterViewModel.h"
#include "Components/Button.h"

void UUDActionParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDActionParameterViewModel* parameterViewModel = CastChecked<UUDActionParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDActionParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	PreviousButtonWidget->OnClicked.Clear();
	NextButtonWidget->OnClicked.Clear();
	PreviousButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionParameterViewModel::PreviousAction);
	NextButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDActionParameterViewModel::NextAction);
}

void UUDActionParameterUserWidget::BindWidgets()
{
	PreviousButtonWidget = GetWidget<UButton>(TEXT("PreviousButton"));
	NextButtonWidget = GetWidget<UButton>(TEXT("NextButton"));
}