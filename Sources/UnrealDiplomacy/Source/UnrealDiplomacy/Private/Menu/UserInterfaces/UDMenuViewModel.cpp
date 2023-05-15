// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDMenuViewModel.h"

#define LOCTEXT_NAMESPACE "Menu"

void UUDMenuViewModel::Update()
{
	FString title = FText(LOCTEXT("Menu", "Koruna")).ToString();
	SetTitle(title);
	FString create = FText(LOCTEXT("Menu", "Create Game")).ToString();
	SetCreateGame(create);
	FString join = FText(LOCTEXT("Menu", "Join Game")).ToString();
	SetJoinGame(join);
	FString settings = FText(LOCTEXT("Menu", "Settings")).ToString();
	SetSettings(settings);
	FString quit = FText(LOCTEXT("Menu", "Quit")).ToString();
	SetQuit(quit);
}

#undef LOCTEXT_NAMESPACE

void UUDMenuViewModel::SetTitle(FString newTitle)
{
	UE_MVVM_SET_PROPERTY_VALUE(Title, newTitle);
}

FString UUDMenuViewModel::GetTitle() const
{
	return Title;
}

void UUDMenuViewModel::SetCreateGame(FString newCreateGame)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreateGame, newCreateGame);
}

FString UUDMenuViewModel::GetCreateGame() const
{
	return CreateGame;
}

void UUDMenuViewModel::SetJoinGame(FString newJoinGame)
{
	UE_MVVM_SET_PROPERTY_VALUE(JoinGame, newJoinGame);
}

FString UUDMenuViewModel::GetJoinGame() const
{
	return JoinGame;
}

void UUDMenuViewModel::SetSettings(FString newSettings)
{
	UE_MVVM_SET_PROPERTY_VALUE(Settings, newSettings);
}

FString UUDMenuViewModel::GetSettings() const
{
	return Settings;
}

void UUDMenuViewModel::SetQuit(FString newQuit)
{
	UE_MVVM_SET_PROPERTY_VALUE(Quit, newQuit);
}

FString UUDMenuViewModel::GetQuit() const
{
	return Quit;
}