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
#include "Core/Simulation/Actions/UDSettingActionWorldRename.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "Lobby"

void UUDLobbyViewModel::Setup()
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
	FText addressTitle = FText(LOCTEXT("Lobby", "Address:"));
	SetConnectAddressTitleText(addressTitle);
	FText address = FText(LOCTEXT("Lobby", "127.0.0.1:7777"));
	SetConnectAddressText(address);
	FText addressHelp = FText(LOCTEXT("Lobby", "Instead of port 0 use 7777 as default port."));
	SetConnectAddressHelpText(addressHelp);

	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	sessions->OnStartSessionCompleteEvent.AddUniqueDynamic(this, &UUDLobbyViewModel::OnSessionStarted);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDLobbyViewModel::Refresh);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDLobbyViewModel::Refresh);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	// Retrieve view models for sub content controls
	TObjectPtr<UUDViewModel> hostModel = hud->GetViewModelCollection(HostViewModelInstanceName, HostViewModelType);
	HostViewModelInstance = Cast<UUDLobbyHostViewModel>(hostModel);
	TObjectPtr<UUDViewModel> memberModel = hud->GetViewModelCollection(MemberViewModelInstanceName, MemberViewModelType);
	MemberViewModelInstance = Cast<UUDLobbyMemberViewModel>(memberModel);
	// Announce them to widget for additional binding.
	SetLobbyHostContent(FUDViewModelContent(HostViewModelInstance));
	SetLobbyMemberContent(FUDViewModelContent(MemberViewModelInstance));
}

void UUDLobbyViewModel::Refresh()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	SetIsHostValue(sessions->IsLocalPlayerHost(sessions->GetSessionName()));
	SetConnectAddressText(FText::FromString(sessions->GetDirectConnectString(sessions->GetSessionName())));
	FText newTitle = FText::Format(LOCTEXT("Lobby", "{0} Lobby"), FText::FromString(sessions->GetSessionNameString()));
	SetLobbyTitleText(newTitle);

	if (!Model->IsOverseeingStatePresent())
		return;
	// Following updates require model.
	UpdateLobbyName();
	UpdateClientList();
	HostViewModelInstance->Refresh();
	MemberViewModelInstance->Refresh();
}

#undef LOCTEXT_NAMESPACE

void UUDLobbyViewModel::UpdateLobbyName()
{
	if (!GetIsHostValue())
	{
		// This updates the client version of display as the session one might be incorrect.
		SetLobbyTitleText(FText::FromString(Model->GetWorldName()));
		return;
	}
	// This only updates the name on server, so it's reflected on all clients.
	FString currentName = GetLobbyTitleText().ToString();
	if (currentName != Model->GetWorldName())
	{
		Model->RequestAction(Model->GetAction(UUDSettingActionWorldRename::ActionTypeId, currentName));
	}
}

void UUDLobbyViewModel::UpdateClientList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyViewModel: UpdateClientList."));
	// Retrieve factions
	TArray<FUDFactionMinimalInfo> factions = Model->GetFactionList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishPlayerController> pc = AUDSkirmishPlayerController::Get(GetWorld());
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(ClientViewModelCollectionName, ClientViewModelType, factions.Num());
	// Get rid of all models
	ClientViewModelCollection.Empty();
	for (int32 i = 0; i < factions.Num(); i++)
	{
		TObjectPtr<UUDClientItemViewModel> newViewModel = Cast<UUDClientItemViewModel>(viewModels[i]);
		newViewModel->SetContent(factions[i]);
		newViewModel->Refresh();
		ClientViewModelCollection.Add(newViewModel);
	}

	SetClientItemList(FUDViewModelList(viewModels));
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

void UUDLobbyViewModel::SetConnectAddressText(FText newConnectAddressText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ConnectAddressText, newConnectAddressText);
}

FText UUDLobbyViewModel::GetConnectAddressText() const
{
	return ConnectAddressText;
}

void UUDLobbyViewModel::SetConnectAddressHelpText(FText newConnectAddressHelpText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ConnectAddressHelpText, newConnectAddressHelpText);
}

FText UUDLobbyViewModel::GetConnectAddressHelpText() const
{
	return ConnectAddressHelpText;
}

void UUDLobbyViewModel::SetConnectAddressTitleText(FText newConnectAddressTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ConnectAddressTitleText, newConnectAddressTitleText);
}

FText UUDLobbyViewModel::GetConnectAddressTitleText() const
{
	return ConnectAddressTitleText;
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

void UUDLobbyViewModel::SetClientItemList(FUDViewModelList newClientItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(ClientItemList, newClientItemList);
}

FUDViewModelList UUDLobbyViewModel::GetClientItemList() const
{
	return ClientItemList;
}

void UUDLobbyViewModel::SetLobbyHostContent(FUDViewModelContent newLobbyHostContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(LobbyHostContent, newLobbyHostContent);
}

FUDViewModelContent UUDLobbyViewModel::GetLobbyHostContent() const
{
	return LobbyHostContent;
}

void UUDLobbyViewModel::SetLobbyMemberContent(FUDViewModelContent newLobbyMemberContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(LobbyMemberContent, newLobbyMemberContent);
}

FUDViewModelContent UUDLobbyViewModel::GetLobbyMemberContent() const
{
	return LobbyMemberContent;
}