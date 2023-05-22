// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDCreateGameViewModel.h"
#include "Menu/UDMenuHUD.h"
#include "Core/UDGlobalData.h"
#include "Core/UDGameInstance.h"
#include "Core/UDSessionSubsystem.h"
#include "OnlineSessionSettings.h"

#define LOCTEXT_NAMESPACE "CreateGame"

void UUDCreateGameViewModel::Initialize()
{
	FText createGameTitle = FText(LOCTEXT("CreateGame", "Create Game"));
	SetCreateGameTitleText(createGameTitle);
	FText back = FText(LOCTEXT("CreateGame", "Back"));
	SetBackText(back);
	FText newGame = FText(LOCTEXT("CreateGame", "New Game"));
	SetNewGameText(newGame);
	FText newSessionName = FText(LOCTEXT("CreateGame", "Session Name"));
	SetSessionNameText(newSessionName);
	FText newSessionNameEditable = FText(LOCTEXT("CreateGame", "Session Name Editable"));
	SetSessionNameEditableText(newSessionNameEditable);
	FText newIsLANText = FText(LOCTEXT("CreateGame", "Is LAN"));
	SetIsLANText(newIsLANText);
}

void UUDCreateGameViewModel::Update()
{
	Load();
}

#undef LOCTEXT_NAMESPACE

void UUDCreateGameViewModel::Back()
{
	UE_LOG(LogTemp, Log, TEXT("UUDCreateGameViewModel: Back."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->MenuScreen);
}

void UUDCreateGameViewModel::NewGame()
{
	UE_LOG(LogTemp, Log, TEXT("UUDCreateGameViewModel: NewGame."));
	CreateNewGame();
}

void UUDCreateGameViewModel::Load()
{
	const FOnlineSessionSettings settings = UUDSessionSubsystem::Get(GetWorld())->GetDefaultSettings();
	SetIsLANValue(UUDApplicationConverters::ToCheckBoxState(settings.bIsLANMatch));
}

void UUDCreateGameViewModel::CreateNewGame()
{
	UUDGameInstance::Get(GetWorld())->OpenSkirmishLevel();
	//UUDSessionSubsystem::Get(GetWorld())->CreateSession(SessionNameText);
}

void UUDCreateGameViewModel::ApplySessionNameChange()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	sessions->SetSessionName(FName(GetSessionNameEditableText().ToString()));
}

void UUDCreateGameViewModel::ApplySessionSettingsChange()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	sessions->ChangeDefaultSettings(UUDApplicationConverters::FromCheckBoxState(GetIsLANValue()));
}

void UUDCreateGameViewModel::SetCreateGameTitleText(FText newCreateGameTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreateGameTitleText, newCreateGameTitleText);
}

FText UUDCreateGameViewModel::GetCreateGameTitleText() const
{
	return CreateGameTitleText;
}

void UUDCreateGameViewModel::SetBackText(FText newBackText)
{
	UE_MVVM_SET_PROPERTY_VALUE(BackText, newBackText);
}

FText UUDCreateGameViewModel::GetBackText() const
{
	return BackText;
}

void UUDCreateGameViewModel::SetNewGameText(FText newNewGameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NewGameText, newNewGameText);
}

FText UUDCreateGameViewModel::GetNewGameText() const
{
	return NewGameText;
}

void UUDCreateGameViewModel::SetSessionNameText(FText newSessionNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SessionNameText, newSessionNameText);
}

FText UUDCreateGameViewModel::GetSessionNameText() const
{
	return SessionNameText;
}

void UUDCreateGameViewModel::SetSessionNameEditableText(FText newSessionNameEditableText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SessionNameEditableText, newSessionNameEditableText);
	ApplySessionNameChange();
}

FText UUDCreateGameViewModel::GetSessionNameEditableText() const
{
	return SessionNameEditableText;
}

void UUDCreateGameViewModel::SetIsLANText(FText newIsLANText)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsLANText, newIsLANText);
}

FText UUDCreateGameViewModel::GetIsLANText() const
{
	return IsLANText;
}

void UUDCreateGameViewModel::SetIsLANValue(ECheckBoxState newIsLANValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsLANValue, newIsLANValue);
	ApplySessionSettingsChange();
}

ECheckBoxState UUDCreateGameViewModel::GetIsLANValue() const
{
	return IsLANValue;
}