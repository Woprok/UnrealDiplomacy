// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDParameterEditorUserWidget.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Components/TileView.h"
#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDActionParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDTileParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDValueParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDTextParameterViewModel.h"

void UUDParameterEditorUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDParameterEditorViewModel* itemViewModel = Cast<UUDParameterEditorViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDParameterEditorUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDParameterEditorViewModel* parameterViewModel = CastChecked<UUDParameterEditorViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDParameterEditorUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->FactionParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetFactionParameterCollection);
	ViewModel->ActionParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetActionParameterCollection);
	ViewModel->TileParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetTileParameterCollection);
	ViewModel->ValueParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetValueParameterCollection);
	ViewModel->TextParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetTextParameterCollection);
	ViewModel->ResourceParameterUpdatedEvent.AddUObject(this, &UUDParameterEditorUserWidget::SetResourceParameterCollection);
	// Bind viewmodel to widgets.
}

void UUDParameterEditorUserWidget::BindWidgets()
{
	FactionParameterListWidget = GetWidget<UTileView>(TEXT("FactionParameterList"));
	TileParameterListWidget = GetWidget<UTileView>(TEXT("TileParameterList"));
	TextParameterListWidget = GetWidget<UTileView>(TEXT("TextParameterList"));
	ResourceParameterListWidget = GetWidget<UTileView>(TEXT("ResourceParameterList"));
	ValueParameterListWidget = GetWidget<UTileView>(TEXT("ValueParameterList"));
	ActionParameterListWidget = GetWidget<UTileView>(TEXT("ActionParameterList"));
}

void UUDParameterEditorUserWidget::SetFactionParameterCollection(const TArray<TObjectPtr<UUDFactionParameterViewModel>>& parameterViewModels)
{
	FactionParameterListWidget->SetListItems(parameterViewModels);
}

void UUDParameterEditorUserWidget::SetActionParameterCollection(const TArray<TObjectPtr<UUDActionParameterViewModel>>& parameterViewModels)
{
	ActionParameterListWidget->SetListItems(parameterViewModels);
}

void UUDParameterEditorUserWidget::SetTileParameterCollection(const TArray<TObjectPtr<UUDTileParameterViewModel>>& parameterViewModels)
{
	TileParameterListWidget->SetListItems(parameterViewModels);
}

void UUDParameterEditorUserWidget::SetValueParameterCollection(const TArray<TObjectPtr<UUDValueParameterViewModel>>& parameterViewModels)
{
	ValueParameterListWidget->SetListItems(parameterViewModels);
}

void UUDParameterEditorUserWidget::SetTextParameterCollection(const TArray<TObjectPtr<UUDTextParameterViewModel>>& parameterViewModels)
{
	TextParameterListWidget->SetListItems(parameterViewModels);
}

void UUDParameterEditorUserWidget::SetResourceParameterCollection(const TArray<TObjectPtr<UUDResourceParameterViewModel>>& parameterViewModels)
{
	ResourceParameterListWidget->SetListItems(parameterViewModels);
}