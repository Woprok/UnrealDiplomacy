// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDMenuUserWidget.h"
#include "Components/Button.h"
#include "Menu/UserInterfaces/UDMenuViewModel.h"
#include "Kismet/GameplayStatics.h"
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
	auto hud = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();
	CastChecked<AUDMenuHUD>(hud)->SwitchScreen("None");
}

void UUDMenuUserWidget::JoinGame()
{
	auto hud = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();
	CastChecked<AUDMenuHUD>(hud)->SwitchScreen("Join");
}

void UUDMenuUserWidget::Settings()
{
	auto hud = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();
	CastChecked<AUDMenuHUD>(hud)->SwitchScreen("Settings");
}

void UUDMenuUserWidget::Quit()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: Quit."));
	CastChecked<UUDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->Quit();
}