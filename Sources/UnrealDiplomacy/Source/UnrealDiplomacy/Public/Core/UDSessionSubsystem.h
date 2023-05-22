// Copyright Miroslav Valach
// Based on as there is nothing new to invent https://cedric-neukirchen.net/docs/session-management/sessions-in-cpp
// Blueprints were easier to setup, but due to almost everything being c++, this was changed as well.
// These are included due to networking.
//#include "Online.h"
//#include "Engine.h"
//#include "Net/UnrealNetwork.h"

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UDSessionSubsystem.generated.h"

// Forward Declarations
class IOnlineSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnDestroySessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUDOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FUDOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

/**
 * Network subsystem for game instance.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/**
	 * Shortcut to retrieve casted SessionSubsystem from GameInstance.
	 * Requries World pointer that can be retrieved by GetWorld().
	 */
	static TObjectPtr<UUDSessionSubsystem> Get(TObjectPtr<UWorld> world);
	/**
	 * Registers delegates for future use.
	 */
	UUDSessionSubsystem();
	/**
	 * Defines Session Paramters that will be used for next session creation.
	 * Nonspecified parameters are based on default session settings.
	 */
	void CreateSettings(FString settingLevelName, int32 numPublicConnections = 32);
	/**
	 * Defines Session Parameters that will be used for next session update.
	 * Based on current session settings.
	 */
	void UpdateSettings(FString settingLevelName);
	/**
	 * Retrieves Session Paramters and returns read-only copy.
	 * Creates new default settings if they are not defined yet.
	 */
	const FOnlineSessionSettings GetDefaultSettings();
	/**
	 * Adjusts specified parameters of default settings. 
	 */
	void ChangeDefaultSettings(bool isLANMatch);
	/**
	 * Defines Session Search Paramters that will be used for next session search.
	 */
	void CreateSearchSettings(int32 maxSearchResults = 20, bool isLANQuery = true);
	/**
	 * Creates new session with specified name and settings specified by last CreateSettings.
	 */
	void CreateSession(FName sessionName);
	/**
	 * Updates session with specified name and settings specified by last UpdateSettings.
	 */
	void UpdateSession(FName sessionName);
	/**
	 * Starts session with specified name.
	 * After this it's expected to travel to another level.
	 */
	void StartSession(FName sessionName);
	/**
	 * Ends session with specified name.
	 * In most cases session is outright destroyed rather then ended.
	 */
	void EndSession(FName sessionName);
	/**
	 * Destroys session with specified name.
	 * Both client and server have to destroy session before creating or joining next.
	 */
	void DestroySession(FName sessionName);
	/**
	 * Find active sessions.
	 */
	void FindSessions();
	/**
	 * Join to specified session that was found by FindSessions.
	 */
	void JoinGameSession(FName sessionName, const FOnlineSessionSearchResult& sessionResult);
	/**
	 * Changes session name to specified value.
	 */
	void SetSessionName(const FName& sessionName);
	/**
	 * Retrieves session name or returns default.
	 */
	FName GetSessionName();
	// Events for controlling session workflow
	FUDOnCreateSessionComplete OnCreateSessionCompleteEvent;
	FUDOnUpdateSessionComplete OnUpdateSessionCompleteEvent;
	FUDOnStartSessionComplete OnStartSessionCompleteEvent;
	FUDOnEndSessionComplete OnEndSessionCompleteEvent;
	FUDOnDestroySessionComplete OnDestroySessionCompleteEvent;
	FUDOnFindSessionsComplete OnFindSessionsCompleteEvent;
	FUDOnJoinSessionComplete OnJoinGameSessionCompleteEvent;
protected:
	/**
	 * SEE ON_JOIN_SUCCESS_TRAVEL THIS IS SAME THING
	 */
	bool TryTravelToCurrentSession(FName sessionName);
	/**
	 * Retrieves UniqueNetId from local player.
	 * TODO check if this can be exposed.
	 */
	bool GetLocalUniqueNetId(FUniqueNetIdRepl& uniqueNetId);
	/**
	 * Attempt to retrieve online subsystem and returns if its valid.
	 */
	bool IsOnlineSubsystemValid(IOnlineSubsystem*& obtainedOnlineSubsystem);
	/**
	 * Attempt to retrieve session interface and returns if its valid.
	 */
	bool IsSessionInterfaceValid(IOnlineSessionPtr& obtainedSessionInterface);
	/**
	 * Returns SessionCount. -1 is returned if pointer is invalid.
	 */
	int32 GetFoundSessionCount();
	/**
	 * Creates default settings.
	 */
	void CreateDefaultSettings();
	/**
	 * Provides default session name from OnlineSessionNames::NAME_GameSession.
	 */
	FName GetDefaultSessionName();
	// Internal Functions for Handles
	void OnCreateSessionCompleted(FName sessionName, bool successful);
	void OnUpdateSessionCompleted(FName sessionName, bool successful);
	void OnStartSessionCompleted(FName sessionName, bool successful);
	void OnEndSessionCompleted(FName sessionName, bool successful);
	void OnDestroySessionCompleted(FName sessionName, bool successful);
	void OnFindSessionsCompleted(bool successful);
	void OnJoinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type result);
private:
	// Settings
	FName CurrentSessionName = TEXT("");
	/**
	 * Settings that are used as base for creating new session settings.
	 */
	TSharedPtr<FOnlineSessionSettings> DefaultSessionSettings;
	/** 
	 * Settings used for next/current session.
	 * TODO current settings tracked per existing session.
	 */
	TSharedPtr<FOnlineSessionSettings> CurrentSessionSettings;
	/**
	 * Settings used for update of current session settings.
	 */
	TSharedPtr<FOnlineSessionSettings> UpdatedSessionSettings;
	TSharedPtr<FOnlineSessionSearch> CurrentSessionSearch;
	// Internal Delegates & Handles
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionHandle;
	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionHandle;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionHandle;
	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionHandle;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionHandle;
};



/*



void UUDMenuUserWidget::Created(bool success)
{

	if (gi)
	{
		sessions->OnCreateSessionCompleteEvent.AddUniqueDynamic(this, &UUDMenuUserWidget::Created);
		sessions->OnUpdateSessionCompleteEvent.AddUniqueDynamic(this, &UUDMenuUserWidget::Updated);
		sessions->OnStartSessionCompleteEvent.AddUniqueDynamic(this, &UUDMenuUserWidget::Started);
		sessions->CreateSettings("TEST");
		sessions->CreateSession(NewSession);
		//sessions->CreateSession(sessions->GetDefaultSessionName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: FAILED."));
	}
	sessions->UpdateSettings("Standard");
	sessions->UpdateSession(NewSession);
	//sessions->UpdateSession(sessions->GetDefaultSessionName());
}

void UUDMenuUserWidget::Updated(bool success)
{
	sessions->StartSession(NewSession);
	//sessions->StartSession(sessions->GetDefaultSessionName());
}
void UUDMenuUserWidget::Started(bool success)
{
	//sessions->EndSession(NewSession);
	//sessions->DestroySession(NewSession);
	//sessions->EndSession(sessions->GetDefaultSessionName());
	//sessions->DestroySession(sessions->GetDefaultSessionName());
}
void UUDMenuUserWidget::Found(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{

	UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: CreateGame."));
	if (gi)
	{
		UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: We have Subsystem."));
		sessions->OnFindSessionsCompleteEvent.AddUObject(this, &UUDMenuUserWidget::Found);
		sessions->CreateSearchSettings();
		sessions->FindSessions();
		//sessions->CreateSession(sessions->GetDefaultSessionName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: FAILED."));
	}

	for (const auto& result : SessionResults)
	{
		sessions->JoinGameSession(NewSession, result);
	}

}
#include "Core/UDSessionSubsystem.h"
	 * Example of travel, if the start succeeded.
void OnStartSuccessTravel()
{
	// If the start was successful, we can open a NewMap if we want. Make sure to
	// use "listen" as a parameter!
	//UGameplayStatics::OpenLevel(GetWorld(), "NewMap", true, "listen");
}
 * Example of travel, if the destroyed succeeded.
void OnDestroySuccessTravel()
{
	//UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap", true);
}
 * Example of travel, if the join succeeded.
void OnJoinSuccessTravel()
{
	// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
	// This is something the Blueprint Node "Join Session" does automatically!
	// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
	// String for us by giving him the SessionName and an empty String. We want to do this, because
	// Every OnlineSubsystem uses different TravelURLs
	//APlayerController* const PlayerController = GetFirstLocalPlayerController();
	//FString TravelURL;
	//if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
	//{
	//	// Finally call the ClienTravel. If you want, you could print the TravelURL to see
	//	// how it really looks like
	//	PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
	//}
}

#include "OnlineSessionSettings.h"
void UUDMenuUserWidget::Created(bool success)
{

	if (gi)
	{
		UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: We have Subsystem."));
		sessions->OnCreateSessionCompleteEvent.AddUniqueDynamic(this, &UUDMenuUserWidget::Created);
		sessions->OnUpdateSessionCompleteEvent.AddUniqueDynamic(this, &UUDMenuUserWidget::Updated);
		sessions->OnStartSessionCompleteEvent.AddUniqueDynamic(this, &UUDMenuUserWidget::Started);
		sessions->CreateSettings("TEST");
		sessions->CreateSession(NewSession);
		//sessions->CreateSession(sessions->GetDefaultSessionName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: FAILED."));
	}
	sessions->UpdateSettings("Standard");
	sessions->UpdateSession(NewSession);
	//sessions->UpdateSession(sessions->GetDefaultSessionName());
}

void UUDMenuUserWidget::Updated(bool success)
{
	sessions->StartSession(NewSession);
	//sessions->StartSession(sessions->GetDefaultSessionName());
}
void UUDMenuUserWidget::Started(bool success)
{
	//sessions->EndSession(NewSession);
	//sessions->DestroySession(NewSession);
	//sessions->EndSession(sessions->GetDefaultSessionName());
	//sessions->DestroySession(sessions->GetDefaultSessionName());
}
void UUDMenuUserWidget::Found(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{

	UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: CreateGame."));
	if (gi)
	{
		UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: We have Subsystem."));
		sessions->OnFindSessionsCompleteEvent.AddUObject(this, &UUDMenuUserWidget::Found);
		sessions->CreateSearchSettings();
		sessions->FindSessions();
		//sessions->CreateSession(sessions->GetDefaultSessionName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UUDMenuUserWidget: FAILED."));
	}

	for (const auto& result : SessionResults)
	{
		sessions->JoinGameSession(NewSession, result);
	}

	UFUNCTION()
		void Created(bool success);
	UFUNCTION()
		void Updated(bool success);
	UFUNCTION()
		void Started(bool success);
	UPROPERTY()
		FName NewSession = TEXT("NEW_SESSION");
	void Found(const TArray<FOnlineSessionSearchResult>&SessionResults, bool Successful);
}*/