// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDTileParameterUserWidget.h"
#include "Skirmish/UserInterfaces/Parameters/UDTileParameterViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDTileParameterUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTileParameterViewModel* parameterViewModel = CastChecked<UUDTileParameterViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTileParameterUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	PreviousButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTileParameterViewModel::PreviousTile);
	NextButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTileParameterViewModel::NextTile);
}

void UUDTileParameterUserWidget::BindWidgets()
{
	TileTitleTextWidget = GetWidget<UTextBlock>(TEXT("TileTitleText"));
	PreviousTextWidget = GetWidget<UTextBlock>(TEXT("PreviousText"));
	NextTextWidget = GetWidget<UTextBlock>(TEXT("NextText"));
	NameTextWidget = GetWidget<UTextBlock>(TEXT("NameText"));
	PreviousButtonWidget = GetWidget<UButton>(TEXT("PreviousButton"));
	NextButtonWidget = GetWidget<UButton>(TEXT("NextButton"));
}