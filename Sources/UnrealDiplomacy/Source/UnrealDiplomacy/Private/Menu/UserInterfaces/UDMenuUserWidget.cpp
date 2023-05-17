// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDMenuUserWidget.h"
#include "Menu/UserInterfaces/UDMenuViewModel.h"
#include "Components/Button.h"
#include "Core/UDGameInstance.h"
#include "Menu/UDMenuHUD.h"

void UUDMenuUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDMenuViewModel* menuViewModel = CastChecked<UUDMenuViewModel>(viewModel.Get());
	ViewModel = menuViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDMenuUserWidget::BindDelegates()
{
	CreateGameButton->OnClicked.AddUniqueDynamic(this, &UUDMenuUserWidget::CreateGame);
	JoinGameButton->OnClicked.AddUniqueDynamic(this, &UUDMenuUserWidget::JoinGame);
	SettingsButton->OnClicked.AddUniqueDynamic(this, &UUDMenuUserWidget::Settings);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UUDMenuUserWidget::Quit);
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