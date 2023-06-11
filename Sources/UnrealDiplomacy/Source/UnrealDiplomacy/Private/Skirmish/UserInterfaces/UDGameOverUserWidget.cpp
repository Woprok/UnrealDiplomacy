// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDGameOverUserWidget.h"
#include "Skirmish/UserInterfaces/UDGameOverViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDGameOverUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDGameOverViewModel* gameOverViewModel = CastChecked<UUDGameOverViewModel>(viewModel.Get());
	ViewModel = gameOverViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDGameOverUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	ReturnButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDGameOverViewModel::Return);
}

void UUDGameOverUserWidget::BindWidgets()
{
	GameOverTitleTextWidget = GetWidget<UTextBlock>(TEXT("GameOverTitleText"));
	WinnerFactionTextWidget = GetWidget<UTextBlock>(TEXT("WinnerFactionText"));
	ReturnToMenuTextWidget = GetWidget<UTextBlock>(TEXT("ReturnToMenuText"));
	ReturnButtonWidget = GetWidget<UButton>(TEXT("ReturnButton"));
}