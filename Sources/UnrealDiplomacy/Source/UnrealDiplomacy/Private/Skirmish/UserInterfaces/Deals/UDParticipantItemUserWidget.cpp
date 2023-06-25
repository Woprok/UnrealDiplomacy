// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDParticipantItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDParticipantItemViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDParticipantItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDParticipantItemViewModel* itemViewModel = Cast<UUDParticipantItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDParticipantItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDParticipantItemViewModel* actionItemViewModel = CastChecked<UUDParticipantItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDParticipantItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDParticipantItemUserWidget::BindWidgets()
{
	FactionNameTextWidget = GetWidget<UTextBlock>(TEXT("FactionNameText"));
	ReadyStateTextWidget = GetWidget<UTextBlock>(TEXT("ReadyStateText"));
	VoteStateTextWidget = GetWidget<UTextBlock>(TEXT("VoteStateText"));
}