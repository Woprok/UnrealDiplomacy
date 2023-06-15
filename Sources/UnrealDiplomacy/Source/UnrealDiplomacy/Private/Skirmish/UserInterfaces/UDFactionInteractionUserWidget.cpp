// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionInteractionUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

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
	// Bind viewmodel to widgets.
	InteractButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDFactionInteractionViewModel::Interact);
}

void UUDFactionInteractionUserWidget::BindWidgets()
{
	NameTextWidget = GetWidget<UTextBlock>(TEXT("NameText"));
	InteractTextWidget = GetWidget<UTextBlock>(TEXT("InteractText"));
	InteractButtonWidget = GetWidget<UButton>(TEXT("InteractButton"));
}