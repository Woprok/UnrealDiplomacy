// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTurnUserWidget.h"
#include "Skirmish/UserInterfaces/UDTurnViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDTurnUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTurnViewModel* turnViewModel = CastChecked<UUDTurnViewModel>(viewModel.Get());
	ViewModel = turnViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTurnUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	FinishTurnButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTurnViewModel::FinishTurn);
}

void UUDTurnUserWidget::BindWidgets()
{
	RegentTitleTextWidget = GetWidget<UTextBlock>(TEXT("RegentTitleText"));
	RegentTextWidget = GetWidget<UTextBlock>(TEXT("RegentText"));
	RemainingTextWidget = GetWidget<UTextBlock>(TEXT("RemainingText"));
	FinishTurnTextWidget = GetWidget<UTextBlock>(TEXT("FinishTurnText"));
	TurnTextWidget = GetWidget<UTextBlock>(TEXT("TurnText"));
	FinishTurnButtonWidget = GetWidget<UButton>(TEXT("FinishTurnButton"));
}