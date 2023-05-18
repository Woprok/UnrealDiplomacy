// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDMenuUserWidget.h"
#include "Menu/UserInterfaces/UDMenuViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Core/UDGameInstance.h"
#include "Menu/UDMenuHUD.h"

void UUDMenuUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
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
	CreateGameButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDMenuUserWidget::CreateGame);
	JoinGameButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDMenuUserWidget::JoinGame);
	SettingsButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDMenuUserWidget::Settings);
	QuitButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDMenuUserWidget::Quit);
}

void UUDMenuUserWidget::CreateGame()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: CreateGame."));
}

void UUDMenuUserWidget::JoinGame()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: JoinGame."));
	AUDMenuHUD::Get(GetWorld())->SwitchScreen("JoinScreen");
}

void UUDMenuUserWidget::Settings()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: Settings."));
	AUDMenuHUD::Get(GetWorld())->SwitchScreen("SettingsScreen");
}

void UUDMenuUserWidget::Quit()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: Quit."));
	UUDGameInstance::Get(GetWorld())->Quit();
}