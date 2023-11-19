// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDMenuViewModel.h"
#include "Core/UDGameInstance.h"
#include "Menu/UDMenuHUD.h"

#define LOCTEXT_NAMESPACE "Menu"

void UUDMenuViewModel::Setup()
{
	Super::Setup();
	FText gameTitle = FText(LOCTEXT("Menu", "Koruna"));
	SetGameTitleText(gameTitle);
	FText createGame = FText(LOCTEXT("Menu", "Create Game"));
	SetCreateGameText(createGame);
	FText joinGame = FText(LOCTEXT("Menu", "Join Game"));
	SetJoinGameText(joinGame);
	FText settings = FText(LOCTEXT("Menu", "Settings"));
	SetSettingsText(settings);
	FText quit = FText(LOCTEXT("Menu", "Quit"));
	SetQuitText(quit);
}

#undef LOCTEXT_NAMESPACE

void UUDMenuViewModel::CreateGame()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMenuViewModel: CreateGame."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->CreateGameScreen);
}

void UUDMenuViewModel::JoinGame()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMenuViewModel: JoinGame."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->JoinGameScreen);
}

void UUDMenuViewModel::Settings()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMenuViewModel: Settings."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->SettingsScreen);
}

void UUDMenuViewModel::Quit()
{
	UE_LOG(LogTemp, Log, TEXT("UUDMenuViewModel: Quit."));
	UUDGameInstance::Get(GetWorld())->Quit();
}

void UUDMenuViewModel::SetGameTitleText(FText newGameTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(GameTitleText, newGameTitleText);
}

FText UUDMenuViewModel::GetGameTitleText() const
{
	return GameTitleText;
}

void UUDMenuViewModel::SetCreateGameText(FText newCreateGameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreateGameText, newCreateGameText);
}

FText UUDMenuViewModel::GetCreateGameText() const
{
	return CreateGameText;
}

void UUDMenuViewModel::SetJoinGameText(FText newJoinGameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(JoinGameText, newJoinGameText);
}

FText UUDMenuViewModel::GetJoinGameText() const
{
	return JoinGameText;
}

void UUDMenuViewModel::SetSettingsText(FText newSettingsText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SettingsText, newSettingsText);
}

FText UUDMenuViewModel::GetSettingsText() const
{
	return SettingsText;
}

void UUDMenuViewModel::SetQuitText(FText newQuitText)
{
	UE_MVVM_SET_PROPERTY_VALUE(QuitText, newQuitText);
}

FText UUDMenuViewModel::GetQuitText() const
{
	return QuitText;
}