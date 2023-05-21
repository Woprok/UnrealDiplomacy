// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDMenuUserWidget.h"
#include "Menu/UserInterfaces/UDMenuViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDMenuUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDMenuViewModel* menuViewModel = CastChecked<UUDMenuViewModel>(viewModel.Get());
	ViewModel = menuViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDMenuUserWidget::BindWidgets()
{
	GameTitleTextWidget = GetWidget<UTextBlock>(TEXT("GameTitleText"));
	CreateGameTextWidget = GetWidget<UTextBlock>(TEXT("CreateGameText"));
	JoinGameTextWidget = GetWidget<UTextBlock>(TEXT("JoinGameText"));
	SettingsTextWidget = GetWidget<UTextBlock>(TEXT("SettingsText"));
	QuitTextWidget = GetWidget<UTextBlock>(TEXT("QuitText"));
	CreateGameButtonWidget = GetWidget<UButton>(TEXT("CreateGameButton"));
	JoinGameButtonWidget = GetWidget<UButton>(TEXT("JoinGameButton"));
	SettingsButtonWidget = GetWidget<UButton>(TEXT("SettingsButton"));
	QuitButtonWidget = GetWidget<UButton>(TEXT("QuitButton"));
}

void UUDMenuUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CreateGameButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMenuViewModel::CreateGame);
	JoinGameButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMenuViewModel::JoinGame);
	SettingsButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMenuViewModel::Settings);
	QuitButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMenuViewModel::Quit);
}