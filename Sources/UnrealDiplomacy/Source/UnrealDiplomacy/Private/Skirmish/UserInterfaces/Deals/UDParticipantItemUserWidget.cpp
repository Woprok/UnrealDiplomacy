// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDParticipantItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDParticipantItemViewModel.h"
#include "Components/Button.h"

void UUDParticipantItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDParticipantItemViewModel* actionItemViewModel = CastChecked<UUDParticipantItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDParticipantItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	KickButtonWidget->OnClicked.Clear();
	KickButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDParticipantItemViewModel::Kick);
}

void UUDParticipantItemUserWidget::BindWidgets()
{
	KickButtonWidget = GetWidget<UButton>(TEXT("KickButton"));
}