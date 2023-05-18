// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDMenuViewModel.h"

#define LOCTEXT_NAMESPACE "Menu"

void UUDMenuViewModel::Update()
{
	FString gameTitle = FText(LOCTEXT("Menu", "Koruna")).ToString();
	SetGameTitleText(gameTitle);
	FString createGame = FText(LOCTEXT("Menu", "Create Game")).ToString();
	SetCreateGameText(createGame);
	FString joinGame = FText(LOCTEXT("Menu", "Join Game")).ToString();
	SetJoinGameText(joinGame);
	FString settings = FText(LOCTEXT("Menu", "Settings")).ToString();
	SetSettingsText(settings);
	FString quit = FText(LOCTEXT("Menu", "Quit")).ToString();
	SetQuitText(quit);
}

#undef LOCTEXT_NAMESPACE

void UUDMenuViewModel::SetGameTitleText(FString newGameTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(GameTitleText, newGameTitleText);
}

FString UUDMenuViewModel::GetGameTitleText() const
{
	return GameTitleText;
}

void UUDMenuViewModel::SetCreateGameText(FString newCreateGameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreateGameText, newCreateGameText);
}

FString UUDMenuViewModel::GetCreateGameText() const
{
	return CreateGameText;
}

void UUDMenuViewModel::SetJoinGameText(FString newJoinGameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(JoinGameText, newJoinGameText);
}

FString UUDMenuViewModel::GetJoinGameText() const
{
	return JoinGameText;
}

void UUDMenuViewModel::SetSettingsText(FString newSettingsText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SettingsText, newSettingsText);
}

FString UUDMenuViewModel::GetSettingsText() const
{
	return SettingsText;
}

void UUDMenuViewModel::SetQuitText(FString newQuitText)
{
	UE_MVVM_SET_PROPERTY_VALUE(QuitText, newQuitText);
}

FString UUDMenuViewModel::GetQuitText() const
{
	return QuitText;
}