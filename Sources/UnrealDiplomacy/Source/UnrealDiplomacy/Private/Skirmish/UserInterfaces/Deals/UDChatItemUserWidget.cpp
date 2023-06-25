// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDChatItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDChatItemViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDChatItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDChatItemViewModel* itemViewModel = Cast<UUDChatItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDChatItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDChatItemViewModel* actionItemViewModel = CastChecked<UUDChatItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDChatItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDChatItemUserWidget::BindWidgets()
{
	FactionTextWidget = GetWidget<UTextBlock>(TEXT("FactionText"));
	MessageTextWidget = GetWidget<UTextBlock>(TEXT("MessageText"));
}