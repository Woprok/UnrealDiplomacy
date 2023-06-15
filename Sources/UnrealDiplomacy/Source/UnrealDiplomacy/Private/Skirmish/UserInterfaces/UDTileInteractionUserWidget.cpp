// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTileInteractionUserWidget.h"
#include "Skirmish/UserInterfaces/UDTileInteractionViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDTileInteractionUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDTileInteractionViewModel* itemViewModel = Cast<UUDTileInteractionViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

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
	NameTextWidget = GetWidget<UTextBlock>(TEXT("NameText"));
	InteractTextWidget = GetWidget<UTextBlock>(TEXT("InteractText"));
	InteractButtonWidget = GetWidget<UButton>(TEXT("InteractButton"));
}