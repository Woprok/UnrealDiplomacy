// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterViewModel.h"
#include "Components/Button.h"

void UUDResourceParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDResourceParameterViewModel* parameterViewModel = CastChecked<UUDResourceParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDResourceParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	PreviousButtonWidget->OnClicked.Clear();
	NextButtonWidget->OnClicked.Clear();

	PreviousButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDResourceParameterViewModel::PreviousResource);
	NextButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDResourceParameterViewModel::NextResource);
}

void UUDResourceParameterUserWidget::BindWidgets()
{
	PreviousButtonWidget = GetWidget<UButton>(TEXT("PreviousButton"));
	NextButtonWidget = GetWidget<UButton>(TEXT("NextButton"));
}