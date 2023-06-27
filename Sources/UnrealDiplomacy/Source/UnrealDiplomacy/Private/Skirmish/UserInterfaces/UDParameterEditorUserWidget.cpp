// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDParameterEditorUserWidget.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDTileParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDActionParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDValueParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDTextParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDTileParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDActionParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDValueParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDTextParameterUserWidget.h"

void UUDParameterEditorUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDParameterEditorViewModel* parameterViewModel = CastChecked<UUDParameterEditorViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDParameterEditorUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->FactionInvokerParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetFactionInvokerParameterInstance);
	ViewModel->FactionTargetParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetFactionTargetParameterInstance);
	ViewModel->ActionParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetActionParameterInstance);
	ViewModel->TileParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetTileParameterInstance);
	ViewModel->ValueParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetValueParameterInstance);
	ViewModel->TextParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetTextParameterInstance);
	ViewModel->ResourceParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetResourceParameterInstance);
	// Bind viewmodel to widgets.
}

void UUDParameterEditorUserWidget::BindWidgets()
{
	FactionInvokerParameterWidget = GetWidget<UUDFactionParameterUserWidget>(TEXT("FactionInvokerParameter"));
	FactionTargetParameterWidget = GetWidget<UUDFactionParameterUserWidget>(TEXT("FactionTargetParameter"));
	TileParameterWidget = GetWidget<UUDTileParameterUserWidget>(TEXT("TileParameter"));
	ActionParameterWidget = GetWidget<UUDActionParameterUserWidget>(TEXT("ActionParameter"));
	ResourceParameterWidget = GetWidget<UUDResourceParameterUserWidget>(TEXT("ResourceParameter"));
	ValueParameterWidget = GetWidget<UUDValueParameterUserWidget>(TEXT("ValueParameter"));
	TextParameterWidget = GetWidget<UUDTextParameterUserWidget>(TEXT("TextParameter"));
}

void UUDParameterEditorUserWidget::SetFactionInvokerParameterInstance(const TObjectPtr<UUDFactionParameterViewModel>& parameterViewModel)
{
	FactionInvokerParameterWidget->SetViewModel(parameterViewModel);
}

void UUDParameterEditorUserWidget::SetFactionTargetParameterInstance(const TObjectPtr<UUDFactionParameterViewModel>& parameterViewModel)
{
	FactionTargetParameterWidget->SetViewModel(parameterViewModel);
}

void UUDParameterEditorUserWidget::SetTileParameterInstance(const TObjectPtr<UUDTileParameterViewModel>& parameterViewModel)
{
	TileParameterWidget->SetViewModel(parameterViewModel);
}

void UUDParameterEditorUserWidget::SetActionParameterInstance(const TObjectPtr<UUDActionParameterViewModel>& parameterViewModel)
{
	ActionParameterWidget->SetViewModel(parameterViewModel);
}

void UUDParameterEditorUserWidget::SetResourceParameterInstance(const TObjectPtr<UUDResourceParameterViewModel>& parameterViewModel)
{
	ResourceParameterWidget->SetViewModel(parameterViewModel);
}

void UUDParameterEditorUserWidget::SetValueParameterInstance(const TObjectPtr<UUDValueParameterViewModel>& parameterViewModel)
{
	ValueParameterWidget->SetViewModel(parameterViewModel);
}

void UUDParameterEditorUserWidget::SetTextParameterInstance(const TObjectPtr<UUDTextParameterViewModel>& parameterViewModel)
{
	TextParameterWidget->SetViewModel(parameterViewModel);
}