// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionItemViewModel.h"
#include "Components/Button.h"

void UUDFactionItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDFactionItemViewModel* itemViewModel = Cast<UUDFactionItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDFactionItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionItemViewModel* factionItemViewModel = CastChecked<UUDFactionItemViewModel>(viewModel.Get());
	ViewModel = factionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	InteractionButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDFactionItemViewModel::Interact);
}

void UUDFactionItemUserWidget::BindWidgets()
{
	InteractionButtonWidget = GetWidget<UButton>(TEXT("InteractionButton"));
}