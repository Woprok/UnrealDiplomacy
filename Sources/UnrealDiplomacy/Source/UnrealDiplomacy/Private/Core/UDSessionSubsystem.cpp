// Copyright Miroslav Valach

#include "Core/UDSessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemUtils.h"
#include "Core/UDGameInstance.h"
#include "Core/UDGlobalData.h"

TObjectPtr<UUDSessionSubsystem> UUDSessionSubsystem::Get(TObjectPtr<UWorld> world)
{
	UE_LOG(LogTemp, Log, TEXT("UUDSessionSubsystem: Retrieving Subsystem from GameInstance."));
	TObjectPtr<UUDGameInstance> gameInstance = UUDGameInstance::Get(world);
	TObjectPtr<UUDSessionSubsystem> sessionSubsystem = gameInstance->GetSubsystem<UUDSessionSubsystem>();
	check(sessionSubsystem != nullptr);
	return sessionSubsystem;
}

UUDSessionSubsystem::UUDSessionSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, 
		&UUDSessionSubsystem::OnCreateSessionCompleted)),
	  UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, 
	 	&UUDSessionSubsystem::OnUpdateSessionCompleted)),
	  StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, 
	 	&UUDSessionSubsystem::OnStartSessionCompleted)),
	  EndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, 
	 	&UUDSessionSubsystem::OnEndSessionCompleted)),
	  DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, 
	 	&UUDSessionSubsystem::OnDestroySessionCompleted)),
	  FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, 
	 	&UUDSessionSubsystem::OnFindSessionsCompleted)),
	  JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, 
		&UUDSessionSubsystem::OnJoinSessionCompleted))
{
	// This needs only to assign delegates.
}

void UUDSessionSubsystem::SetSessionName(const FString& sessionName)
{
	CurrentSessionName = sessionName;
}

FName UUDSessionSubsystem::GetSessionName()
{
	if (CurrentSessionName.IsEmpty())
		return GetDefaultSessionName();
	return FName(CurrentSessionName);
}

FString UUDSessionSubsystem::GetSessionNameString()
{
	if (CurrentSessionName.IsEmpty())
		return GetDefaultSessionName().ToString();
	return CurrentSessionName;
}

void UUDSessionSubsystem::CreateSettings(FString settingLevelName, int32 numPublicConnections)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Settings new level %s."), *settingLevelName);
	CurrentSessionSettings = MakeShareable(new FOnlineSessionSettings(*DefaultSessionSettings));
	CurrentSessionSettings->NumPublicConnections = numPublicConnections;
	// This has lot more options...
	CurrentSessionSettings->Set(SETTING_MAPNAME, settingLevelName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	CurrentSessionSettings->Set(UD_SETTING_SESSIONNAME, GetSessionNameString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
}

void UUDSessionSubsystem::UpdateSettings(FString settingLevelName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Settings updated level %s."), *settingLevelName);
	UpdatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*CurrentSessionSettings));
	// This has lot more options...
	UpdatedSessionSettings->Set(SETTING_MAPNAME, settingLevelName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	UpdatedSessionSettings->Set(UD_SETTING_SESSIONNAME, GetSessionNameString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
}

void UUDSessionSubsystem::CreateDefaultSettings()
{
	DefaultSessionSettings = MakeShareable(new FOnlineSessionSettings());
	DefaultSessionSettings->NumPrivateConnections = 0;
	DefaultSessionSettings->NumPublicConnections = 32;
	DefaultSessionSettings->bAllowInvites = true;
	DefaultSessionSettings->bAllowJoinInProgress = true;
	DefaultSessionSettings->bAllowJoinViaPresence = true;
	DefaultSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	DefaultSessionSettings->bIsDedicated = false;
	DefaultSessionSettings->bUsesPresence = true;
	DefaultSessionSettings->bIsLANMatch = false;
	DefaultSessionSettings->bShouldAdvertise = true;
	// This has lot more options...
	DefaultSessionSettings->Set(SETTING_MAPNAME, UUDGlobalData::DefaultLevelName(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	DefaultSessionSettings->Set(UD_SETTING_SESSIONNAME, GetSessionNameString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
}

void UUDSessionSubsystem::ChangeDefaultSettings(bool isLANMatch)
{
	DefaultSessionSettings->bIsLANMatch = isLANMatch;
}

const FOnlineSessionSettings UUDSessionSubsystem::GetDefaultSettings()
{
	if (DefaultSessionSettings == nullptr)
		CreateDefaultSettings();
	return FOnlineSessionSettings(*DefaultSessionSettings);
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

void UUDSessionSubsystem::CreateSearchSettings(int32 maxSearchResults, bool isLANQuery)
{
	CurrentSessionSearch = MakeShareable(new FOnlineSessionSearch());
	CurrentSessionSearch->MaxSearchResults = maxSearchResults;
	CurrentSessionSearch->bIsLanQuery = isLANQuery;
	CurrentSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
}

int32 UUDSessionSubsystem::GetFoundSessionCount()
{
	if (CurrentSessionSearch)
		return CurrentSessionSearch->SearchResults.Num();
	return -1;
}

void UUDSessionSubsystem::CreateSession(FName sessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Creating %s."), *sessionName.ToString());
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
	if (!sessionInterface->CreateSession(*localUniqueNetId, sessionName, *CurrentSessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void UUDSessionSubsystem::UpdateSession(FName sessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Updating %s."), *sessionName.ToString());
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
	if (!sessionInterface->UpdateSession(sessionName, *UpdatedSessionSettings))
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionHandle);
		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		CurrentSessionSettings = UpdatedSessionSettings;
	}
}

void UUDSessionSubsystem::StartSession(FName sessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Starting %s."), *sessionName.ToString());
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
	if (!sessionInterface->StartSession(sessionName))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionHandle);
		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void UUDSessionSubsystem::EndSession(FName sessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Ending %s."), *sessionName.ToString());
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
	if (!sessionInterface->EndSession(sessionName))
	{
		sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionHandle);
		OnEndSessionCompleteEvent.Broadcast(false);
	}
}

void UUDSessionSubsystem::DestroySession(FName sessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Destroying %s."), *sessionName.ToString());
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
	if (!sessionInterface->DestroySession(sessionName))
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

void UUDSessionSubsystem::JoinGameSession(FName sessionName, const FOnlineSessionSearchResult& sessionResult)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Joining %s."), *sessionName.ToString());
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		OnJoinGameSessionCompleteEvent.Broadcast(sessionName, EOnJoinSessionCompleteResult::UnknownError);
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
	if (!sessionInterface->JoinSession(*localUniqueNetId, sessionName, sessionResult))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionHandle);
		OnJoinGameSessionCompleteEvent.Broadcast(sessionName, EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UUDSessionSubsystem::OnCreateSessionCompleted(FName sessionName, bool successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnCreate. %s -> %d."), *sessionName.ToString(), successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
	}
	// Propagate.
	OnCreateSessionCompleteEvent.Broadcast(successful);
}

void UUDSessionSubsystem::OnUpdateSessionCompleted(FName sessionName, bool successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnUpdate. %s -> %d."), *sessionName.ToString(), successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionHandle);
	}
	// Propagate.
	OnUpdateSessionCompleteEvent.Broadcast(successful);
}

void UUDSessionSubsystem::OnStartSessionCompleted(FName sessionName, bool successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnStart. %s -> %d."), *sessionName.ToString(), successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionHandle);
	}
	// Propagate.
	OnStartSessionCompleteEvent.Broadcast(successful);
}

void UUDSessionSubsystem::OnEndSessionCompleted(FName sessionName, bool successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnEnd. %s -> %d."), *sessionName.ToString(), successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionHandle);
	}
	// Propagate.
	OnEndSessionCompleteEvent.Broadcast(successful);
}

void UUDSessionSubsystem::OnDestroySessionCompleted(FName sessionName, bool successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnDestroy. %s -> %d."), *sessionName.ToString(), successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionHandle);
	}
	// Propagate.
	OnDestroySessionCompleteEvent.Broadcast(successful);
}

void UUDSessionSubsystem::OnJoinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnJoin. %s -> %d."), *sessionName.ToString(), result);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionHandle);
	}
	// Propagate.
	OnJoinGameSessionCompleteEvent.Broadcast(sessionName, result);
}

void UUDSessionSubsystem::OnFindSessionsCompleted(bool successful)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: OnFind. %d."), GetFoundSessionCount(), successful);
	IOnlineSessionPtr sessionInterface;
	// Unsubscribe.
	if (IsSessionInterfaceValid(sessionInterface))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsHandle);
	}
	// Propagate based on result count.
	if (GetFoundSessionCount() > 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(CurrentSessionSearch->SearchResults, successful);
	}
	else
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), successful);
	}
}

bool UUDSessionSubsystem::TryTravelToCurrentSession(FName sessionName)
{
	UE_LOG(LogTemp, Log, TEXT("Sessions: Traveling %s."), *sessionName.ToString());
	// Check Session Interface.
	IOnlineSessionPtr sessionInterface;
	if (!IsSessionInterfaceValid(sessionInterface))
	{
		return false;
	}
	// Retrieves connection / travel url.
	FString connectString;
	if (!sessionInterface->GetResolvedConnectString(sessionName, connectString))
	{
		return false;
	}
	// Travels to server map.
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
	return true;
}