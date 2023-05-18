// Copyright Miroslav Valach

#include "Core/UDSessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemUtils.h"
// These are included due to networking.
//#include "Online.h"
//#include "Engine.h"
//#include "Net/UnrealNetwork.h"

UUDSessionSubsystem::UUDSessionSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, 
		&UUDSessionSubsystem::OnCreateSessionCompleted))
	, UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, 
		&UUDSessionSubsystem::OnUpdateSessionCompleted))
	, StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, 
		&UUDSessionSubsystem::OnStartSessionCompleted))
	, EndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, 
		&UUDSessionSubsystem::OnEndSessionCompleted))
	, DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, 
		&UUDSessionSubsystem::OnDestroySessionCompleted))
	, FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, 
		&UUDSessionSubsystem::OnFindSessionsCompleted))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, 
		&UUDSessionSubsystem::OnJoinSessionCompleted))
{
	// This needs only to assign delegates.
}

void UUDSessionSubsystem::CreateSettings(FString SettingLevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	CurrentSessionSettings = MakeShareable(new FOnlineSessionSettings());
	CurrentSessionSettings->NumPrivateConnections = 0;
	CurrentSessionSettings->NumPublicConnections = NumPublicConnections;
	CurrentSessionSettings->bAllowInvites = true;
	CurrentSessionSettings->bAllowJoinInProgress = true;
	CurrentSessionSettings->bAllowJoinViaPresence = true;
	CurrentSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	CurrentSessionSettings->bIsDedicated = false;
	CurrentSessionSettings->bUsesPresence = true;
	CurrentSessionSettings->bIsLANMatch = IsLANMatch;
	CurrentSessionSettings->bShouldAdvertise = true;
	// This has lot more options...
	CurrentSessionSettings->Set(SETTING_MAPNAME, SettingLevelName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
}

void UUDSessionSubsystem::UpdateSettings(FString SettingLevelName)
{
	UpdatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*CurrentSessionSettings));
	// This has lot more options...
	UpdatedSessionSettings->Set(SETTING_MAPNAME, SettingLevelName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
}

FName UUDSessionSubsystem::GetDefaultSessionName()
{
	return NAME_GameSession;
}

bool UUDSessionSubsystem::GetLocalUniqueNetId(FUniqueNetIdRepl& uniqueNetId)
{
	// There is an alternative that ignores the world GetFirstGamePlayer.
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!localPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Sessions: Local Player Error."));
		return false;
	}

	uniqueNetId = localPlayer->GetPreferredUniqueNetId();
	return true;
}

bool UUDSessionSubsystem::IsOnlineSubsystemValid(IOnlineSubsystem*& obtainedOnlineSubsystem)
{
	obtainedOnlineSubsystem = IOnlineSubsystem::Get();
	if (!obtainedOnlineSubsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("Sessions: Online Subsystem Error."));
		return false;
	}
	return true;
}

bool UUDSessionSubsystem::IsSessionInterfaceValid(IOnlineSessionPtr& obtainedSessionInterface)
{
	// TODO use online subsystem ? IOnlineSubsystem::Get()->GetSessionInterface()
	obtainedSessionInterface = Online::GetSessionInterface(GetWorld());
	if (!obtainedSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Sessions: Session Interface Error."));
		return false;
	}
	return true;
}

void UUDSessionSubsystem::CreateSearchSettings(int32 MaxSearchResults, bool IsLANQuery)
{
	CurrentSessionSearch = MakeShareable(new FOnlineSessionSearch());
	CurrentSessionSearch->MaxSearchResults = MaxSearchResults;
	CurrentSessionSearch->bIsLanQuery = IsLANQuery;
	CurrentSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
}

int32 UUDSessionSubsystem::GetFoundSessionCount()
{
	if (CurrentSessionSearch)
		return CurrentSessionSearch->SearchResults.Num();
	return -1;
}

void UUDSessionSubsystem::CreateSession(FName SessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Creating %s."), SessionName);
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}
	// Check Local Player and get his UniqueNetId.
	FUniqueNetIdRepl localUniqueNetId;
	if (!GetLocalUniqueNetId(localUniqueNetId))
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}
	// Subscribe Handle.
	CreateSessionHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	// Finally execute creation.
	if (!sessionInterface->CreateSession(*localUniqueNetId, SessionName, *CurrentSessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void UUDSessionSubsystem::UpdateSession(FName SessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Updating %s."), SessionName);
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}
	// Subscribe Handle.
	UpdateSessionHandle = sessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);
	// Finally execute update.
	if (!sessionInterface->UpdateSession(SessionName, *UpdatedSessionSettings))
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionHandle);
		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		CurrentSessionSettings = UpdatedSessionSettings;
	}
}

void UUDSessionSubsystem::StartSession(FName SessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Starting %s."), SessionName);
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		OnStartSessionCompleteEvent.Broadcast(false);
		return;
	}
	// Subscribe Handle.
	StartSessionHandle = sessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);
	// Finally execute start.
	if (!sessionInterface->StartSession(NAME_GameSession))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionHandle);
		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void UUDSessionSubsystem::EndSession(FName SessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Ending %s."), SessionName);
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		OnEndSessionCompleteEvent.Broadcast(false);
		return;
	}
	// Subscribe Handle.
	EndSessionHandle = sessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);
	// Finally execute end.
	if (!sessionInterface->EndSession(NAME_GameSession))
	{
		sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionHandle);
		OnEndSessionCompleteEvent.Broadcast(false);
	}
}

void UUDSessionSubsystem::DestroySession(FName SessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Destroying %s."), SessionName);
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		OnDestroySessionCompleteEvent.Broadcast(false);
		return;
	}
	// Subscribe Handle.
	DestroySessionHandle = sessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
	// Finally execute destroy.
	if (!sessionInterface->DestroySession(NAME_GameSession))
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionHandle);
		OnDestroySessionCompleteEvent.Broadcast(false);
	}
}

void UUDSessionSubsystem::FindSessions()
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Finding."));
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	// Check Local Player and get his UniqueNetId.
	FUniqueNetIdRepl localUniqueNetId;
	if (!GetLocalUniqueNetId(localUniqueNetId))
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}	
	// Subscribe Handle.
	FindSessionsHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	// Finally execute search.
	if (!sessionInterface->FindSessions(*localUniqueNetId, CurrentSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsHandle);
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UUDSessionSubsystem::JoinGameSession(FName SessionName, const FOnlineSessionSearchResult& SessionResult)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Joining %s."), SessionName);
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}
	// Check Local Player and get his UniqueNetId.
	FUniqueNetIdRepl localUniqueNetId;
	if (!GetLocalUniqueNetId(localUniqueNetId))
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}
	// Subscribe Handle.
	JoinSessionHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
	// Finally execute join.
	if (!sessionInterface->JoinSession(*localUniqueNetId, SessionName, SessionResult))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionHandle);
		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UUDSessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnCreate. %s -> %d."), SessionName, Successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
	}
	// Propagate.
	OnCreateSessionCompleteEvent.Broadcast(Successful);
}

void UUDSessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnUpdate. %s -> %d."), SessionName, Successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionHandle);
	}
	// Propagate.
	OnUpdateSessionCompleteEvent.Broadcast(Successful);
}

void UUDSessionSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnStart. %s -> %d."), SessionName, Successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionHandle);
	}
	// Propagate.
	OnStartSessionCompleteEvent.Broadcast(Successful);
}

void UUDSessionSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnEnd. %s -> %d."), SessionName, Successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionHandle);
	}
	// Propagate.
	OnEndSessionCompleteEvent.Broadcast(Successful);
}

void UUDSessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnDestroy. %s -> %d."), SessionName, Successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionHandle);
	}
	// Propagate.
	OnDestroySessionCompleteEvent.Broadcast(Successful);
}

void UUDSessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnJoin. %s -> %d."), SessionName, Result);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionHandle);
	}
	// Propagate.
	OnJoinGameSessionCompleteEvent.Broadcast(Result);
}

void UUDSessionSubsystem::OnFindSessionsCompleted(bool Successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnFind. %d."), GetFoundSessionCount(), Successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsHandle);
	}
	// Propagate based on result count.
	if (GetFoundSessionCount() > 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(CurrentSessionSearch->SearchResults, Successful);
	}
	else
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
	}
}

bool UUDSessionSubsystem::TryTravelToCurrentSession(FName SessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Traveling %s."), SessionName);
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		return false;
	}
	// Retrieves connection / travel url.
	FString connectString;
	if (!sessionInterface->GetResolvedConnectString(SessionName, connectString))
	{
		return false;
	}
	// Travels to server map.
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
	return true;
}