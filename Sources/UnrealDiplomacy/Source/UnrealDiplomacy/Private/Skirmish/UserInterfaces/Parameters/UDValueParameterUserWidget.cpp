// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDValueParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDValueParameterViewModel.h"
#include "Components/SpinBox.h"

void UUDValueParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDValueParameterViewModel* parameterViewModel = CastChecked<UUDValueParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDValueParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	ValueBoxWidget->OnValueChanged.AddUniqueDynamic(ViewModel.Get(), &UUDValueParameterViewModel::StartValueEditation);
	ValueBoxWidget->OnValueCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDValueParameterViewModel::StopValueEditation);
}

void UUDValueParameterUserWidget::BindWidgets()
{
	ValueBoxWidget = GetWidget<USpinBox>(TEXT("ValueBox"));
}