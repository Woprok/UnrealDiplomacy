// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDMenuUserWidget.h"
#include "Menu/UserInterfaces/UDMenuViewModel.h"
#include "Components/Button.h"

void UUDMenuUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDMenuViewModel* menuViewModel = CastChecked<UUDMenuViewModel>(viewModel.Get());
	ViewModel = menuViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDMenuUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CreateGameButtonWidget->OnClicked.Clear();
	JoinGameButtonWidget->OnClicked.Clear();
	SettingsButtonWidget->OnClicked.Clear();
	QuitButtonWidget->OnClicked.Clear();

	CreateGameButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMenuViewModel::CreateGame);
	JoinGameButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMenuViewModel::JoinGame);
	SettingsButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMenuViewModel::Settings);
	QuitButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDMenuViewModel::Quit);
}

void UUDMenuUserWidget::BindWidgets()
{
	CreateGameButtonWidget = GetWidget<UButton>(TEXT("CreateGameButton"));
	JoinGameButtonWidget = GetWidget<UButton>(TEXT("JoinGameButton"));
	SettingsButtonWidget = GetWidget<UButton>(TEXT("SettingsButton"));
	QuitButtonWidget = GetWidget<UButton>(TEXT("QuitButton"));
}