// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyViewModel.h"
#include "Core/UDGameInstance.h"
#include "Core/UDSessionSubsystem.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "Lobby"

void UUDLobbyViewModel::Initialize()
{
	FText lobbyTitle = FText(LOCTEXT("Lobby", "Lobby"));
	SetLobbyTitleText(lobbyTitle);
	FText nameHeader = FText(LOCTEXT("Lobby", "Name"));
	SetNameHeaderText(nameHeader);
	FText back = FText(LOCTEXT("Lobby", "Back"));
	SetBackText(back);
	FText leave = FText(LOCTEXT("Lobby", "Leave"));
	SetLeaveText(leave);
	FText start = FText(LOCTEXT("Lobby", "Start"));
	SetStartText(start);

	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	sessions->OnStartSessionCompleteEvent.AddUniqueDynamic(this, &UUDLobbyViewModel::OnSessionStarted);
}

void UUDLobbyViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDLobbyViewModel::Back()
{
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: Back."));
	// Back is from Host perspective.
	QuitGame();
}

void UUDLobbyViewModel::Leave()
{
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: Leave."));
	// Leave is from Client perspective.
	LeaveGame();
}

void UUDLobbyViewModel::Start()
{
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: Start."));
	StartGame();
}

void UUDLobbyViewModel::StartGame()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: Starting session name %s."), *sessions->GetSessionNameString());

	sessions->StartSession(sessions->GetSessionName());
}

void UUDLobbyViewModel::QuitGame()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: Host quit the game -> %s."), *sessions->GetSessionNameString());

	sessions->QuitSession();
}

void UUDLobbyViewModel::LeaveGame()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: Client left the game -> %s."), *sessions->GetSessionNameString());

	sessions->LeaveSession();

}

void UUDLobbyViewModel::OnSessionStarted(bool success)
{
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: OnSessionStarted."));
	// Session was started, we can proceed to different UI and call GameMode to handle rest.
}

void UUDLobbyViewModel::SetLobbyTitleText(FText newLobbyTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(LobbyTitleText, newLobbyTitleText);
}

FText UUDLobbyViewModel::GetLobbyTitleText() const
{
	return LobbyTitleText;
}

void UUDLobbyViewModel::SetNameHeaderText(FText newNameHeaderText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameHeaderText, newNameHeaderText);
}

FText UUDLobbyViewModel::GetNameHeaderText() const
{
	return NameHeaderText;
}

void UUDLobbyViewModel::SetBackText(FText newBackText)
{
	UE_MVVM_SET_PROPERTY_VALUE(BackText, newBackText);
}

FText UUDLobbyViewModel::GetBackText() const
{
	return BackText;
}

void UUDLobbyViewModel::SetLeaveText(FText newLeaveText)
{
	UE_MVVM_SET_PROPERTY_VALUE(LeaveText, newLeaveText);
}

FText UUDLobbyViewModel::GetLeaveText() const
{
	return LeaveText;
}

void UUDLobbyViewModel::SetStartText(FText newStartText)
{
	UE_MVVM_SET_PROPERTY_VALUE(StartText, newStartText);
}

FText UUDLobbyViewModel::GetStartText() const
{
	return StartText;
}