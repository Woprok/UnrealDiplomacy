// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyViewModel.h"
#include "Lobby/UserInterfaces/UDLobbyHostViewModel.h"
#include "Lobby/UserInterfaces/UDLobbyMemberViewModel.h"
#include "Lobby/UserInterfaces/UDClientItemViewModel.h"
#include "Core/UDGameInstance.h"
#include "Core/UDSessionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Simulation/UDCommandData.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "Lobby"

void UUDLobbyViewModel::Initialize()
{
	HostViewModelType = UUDLobbyHostViewModel::StaticClass();
	MemberViewModelType = UUDLobbyMemberViewModel::StaticClass();
	ClientViewModelType = UUDClientItemViewModel::StaticClass();

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

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDLobbyViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDLobbyViewModel::Update);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	// Retrieve view models for sub content controls
	TObjectPtr<UUDViewModel> hostModel = hud->GetViewModelCollection(HostViewModelInstanceName, HostViewModelType);
	HostViewModelInstance = Cast<UUDLobbyHostViewModel>(hostModel);
	TObjectPtr<UUDViewModel> memberModel = hud->GetViewModelCollection(MemberViewModelInstanceName, MemberViewModelType);
	MemberViewModelInstance = Cast<UUDLobbyMemberViewModel>(memberModel);
	// Announce them to widget for additional binding.
	LobbyHostSourceChangedEvent.Broadcast(HostViewModelInstance);
	LobbyMemberSourceChangedEvent.Broadcast(MemberViewModelInstance);
	// Call initialize so each Instance is ready to use, once it receives data in runtime.
	HostViewModelInstance->FullUpdate();
	MemberViewModelInstance->FullUpdate();

	Update();
}

void UUDLobbyViewModel::Reload()
{
	Update();
}

void UUDLobbyViewModel::Update()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	SetIsHostValue(sessions->IsLocalPlayerHost(sessions->GetSessionName()));

	FText newTitle = FText::Format(LOCTEXT("Lobby", "{0} Lobby"), FText::FromString(sessions->GetSessionNameString()));
	SetLobbyTitleText(newTitle);

	if (!Model->IsOverseeingStatePresent())
		return;
	// Following updates require model.
	UpdateClientList();
}

#undef LOCTEXT_NAMESPACE

void UUDLobbyViewModel::UpdateClientList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: UpdateClients."));
	// Retrieve factions
	TArray<FUDFactionMinimalInfo> factions = Model->GetFactionList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishPlayerController> pc = AUDSkirmishPlayerController::Get(GetWorld());
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ClientViewModelCollectionName, ClientViewModelType, factions.Num());
	// Get rid of all models
	ClientViewModelCollection.Empty();
	for (int32 i = 0; i < factions.Num(); i++)
	{
		TObjectPtr<UUDClientItemViewModel> newViewModel = Cast<UUDClientItemViewModel>(viewModels[i]);
		bool isHost = factions[i].Id == pc->GetControlledFactionId();
		newViewModel->SetContent(factions[i]);
		newViewModel->FullUpdate();
		ClientViewModelCollection.Add(newViewModel);
	}
	
	LobbyClientSourceUpdatedEvent.Broadcast(ClientViewModelCollection);
}

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
	AUDSkirmishPlayerController::Get(GetWorld())->OnUserCommandRequested(FUDCommandData(EUDCommandType::HostClosedGame));

	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: Host quit the game -> %s."), *sessions->GetSessionNameString());
	sessions->QuitSession();
}

void UUDLobbyViewModel::LeaveGame()
{
	AUDSkirmishPlayerController::Get(GetWorld())->OnUserCommandRequested(FUDCommandData(EUDCommandType::PlayerLeftGame));

	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: Client left the game -> %s."), *sessions->GetSessionNameString());
	sessions->LeaveSession();
}

void UUDLobbyViewModel::OnSessionStarted(bool success)
{
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: OnSessionStarted."));
	// Session was started, we can proceed to different UI and call GameMode to handle rest.
	AUDSkirmishPlayerController::Get(GetWorld())->OnUserCommandRequested(FUDCommandData(EUDCommandType::StartGame));
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

void UUDLobbyViewModel::SetIsHostValue(bool newIsHostValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsHostValue, newIsHostValue);
}

bool UUDLobbyViewModel::GetIsHostValue() const
{
	return IsHostValue;
}