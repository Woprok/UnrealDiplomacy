// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTileInteractionUserWidget.h"
#include "Skirmish/UserInterfaces/UDTileInteractionViewModel.h"
#include "Components/Button.h"

void UUDTileInteractionUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTileInteractionViewModel* tileInteractionViewModel = CastChecked<UUDTileInteractionViewModel>(viewModel.Get());
	ViewModel = tileInteractionViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTileInteractionUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	InteractButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTileInteractionViewModel::Interact);
}

void UUDTileInteractionUserWidget::BindWidgets()
{
	InteractButtonWidget = GetWidget<UButton>(TEXT("InteractButton"));
}