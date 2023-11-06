// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPointContentUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorUserWidget.h"
#include "Components/Button.h"

void UUDPointContentUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPointContentViewModel* actionItemViewModel = CastChecked<UUDPointContentViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDPointContentUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->ParameterEditorChangedEvent.AddUObject(this, &UUDPointContentUserWidget::SetParameterEditorSourceInstance);
	// Bind viewmodel to widgets.
}

void UUDPointContentUserWidget::BindWidgets()
{
	ParameterEditorViewWidget = GetWidget<UUDParameterEditorUserWidget>(TEXT("ParameterEditorView"));
}

void UUDPointContentUserWidget::SetParameterEditorSourceInstance(const TObjectPtr<UUDParameterEditorViewModel>& parameterEditorViewModel)
{
	ParameterEditorViewWidget->SetViewModel(parameterEditorViewModel);
}