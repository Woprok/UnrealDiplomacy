// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDJoinGameUserWidget.h"
#include "Menu/UserInterfaces/UDJoinGameViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDJoinGameUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDJoinGameViewModel* joinGameViewModel = CastChecked<UUDJoinGameViewModel>(viewModel.Get());
	ViewModel = joinGameViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDJoinGameUserWidget::BindWidgets()
{
	JoinGameTitleTextWidget = GetWidget<UTextBlock>(TEXT("JoinGameTitleText"));
	BackTextWidget = GetWidget<UTextBlock>(TEXT("BackText"));
	RefreshTextWidget = GetWidget<UTextBlock>(TEXT("RefreshText"));
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
}

void UUDJoinGameUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	BackButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDJoinGameViewModel::Back);
}