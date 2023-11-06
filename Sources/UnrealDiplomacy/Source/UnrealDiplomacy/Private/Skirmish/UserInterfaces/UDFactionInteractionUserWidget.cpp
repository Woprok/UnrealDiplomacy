// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionInteractionUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorUserWidget.h"
#include "Components/Button.h"

void UUDFactionInteractionUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDFactionInteractionViewModel* itemViewModel = Cast<UUDFactionInteractionViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDFactionInteractionUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionInteractionViewModel* factionInteractionViewModel = CastChecked<UUDFactionInteractionViewModel>(viewModel.Get());
	ViewModel = factionInteractionViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionInteractionUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->ParameterEditorChangedEvent.AddUObject(this, &UUDFactionInteractionUserWidget::SetParameterEditorSourceInstance);
	// Bind viewmodel to widgets.
	InteractButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDFactionInteractionViewModel::Interact);
}

void UUDFactionInteractionUserWidget::BindWidgets()
{
	InteractButtonWidget = GetWidget<UButton>(TEXT("InteractButton"));
	ParameterEditorViewWidget = GetWidget<UUDParameterEditorUserWidget>(TEXT("ParameterEditorView"));
}

void UUDFactionInteractionUserWidget::SetParameterEditorSourceInstance(const TObjectPtr<UUDParameterEditorViewModel>& parameterEditorViewModel)
{
	ParameterEditorViewWidget->SetViewModel(parameterEditorViewModel);
}