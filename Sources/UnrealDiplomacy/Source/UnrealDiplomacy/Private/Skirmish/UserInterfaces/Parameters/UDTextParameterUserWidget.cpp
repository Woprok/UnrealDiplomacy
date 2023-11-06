// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDTextParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDTextParameterViewModel.h"
#include "Components/EditableTextBox.h"

void UUDTextParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTextParameterViewModel* parameterViewModel = CastChecked<UUDTextParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTextParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	TextEditableTextWidget->OnTextChanged.AddUniqueDynamic(ViewModel.Get(), &UUDTextParameterViewModel::StartTextEditation);
	TextEditableTextWidget->OnTextCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDTextParameterViewModel::StopTextEditation);
}

void UUDTextParameterUserWidget::BindWidgets()
{
	TextEditableTextWidget = GetWidget<UEditableTextBox>(TEXT("TextEditableText"));
}