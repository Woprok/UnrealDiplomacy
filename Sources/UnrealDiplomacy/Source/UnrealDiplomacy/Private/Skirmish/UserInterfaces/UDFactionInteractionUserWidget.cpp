// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionInteractionUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Components/Button.h"

void UUDFactionInteractionUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionInteractionViewModel* factionInteractionViewModel = CastChecked<UUDFactionInteractionViewModel>(viewModel.Get());
	ViewModel = factionInteractionViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionInteractionUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	InteractButtonWidget->OnClicked.Clear();
	InteractButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDFactionInteractionViewModel::Interact);
}

void UUDFactionInteractionUserWidget::BindWidgets()
{
	InteractButtonWidget = GetWidget<UButton>(TEXT("InteractButton"));
}