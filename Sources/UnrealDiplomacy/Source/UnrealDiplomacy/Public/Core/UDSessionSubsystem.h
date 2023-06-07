// Copyright Miroslav Valach
// Based on https://cedric-neukirchen.net/docs/session-management/sessions-in-cpp

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UDSessionSubsystem.generated.h"

// Forward Declarations

class IOnlineSubsystem;

#define UD_SETTING_SESSIONNAME FName(TEXT("UD_SESSIONNAME"))

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnDestroySessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUDOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUDOnJoinSessionComplete, FName SessionName, EOnJoinSessionCompleteResult::Type Result);

/**
 * Network subsystem for game instance.
 */
UCLASS(Blueprintable, BlueprintType)
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
	 * Attempts to join same world as host.
	 */
	bool TryTravelToCurrentSession(FName sessionName);
	/**
	 * Wrapper around destroy, disconnect and return for client.
	 * Client requested to leave session and level. 
	 * Leaves current game.
	 */
	void LeaveSession();
	/**
	 * Wrapper around destroy, disconnect and return for host.
	 * Host requested for this session and level to be destroyed.
	 * All players will be disconnected and returned to menu.
	 * Clients will most likely have to handle network error.
	 * End current game.
	 */
	void QuitSession();
	/**
	 * Changes session name to specified value.
	 */
	void SetSessionName(const FString& sessionName);
	/**
	 * Retrieves stored session name or returns default.
	 * This is converted value from FString.
	 */
	FName GetSessionName();
	/**
	 * Retrieves stored session name or returns default.
	 */
	FString GetSessionNameString();
	/**
	 * Checks if local player is host of the current session. 
	 */
	bool IsLocalPlayerHost(FName sessionName);
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
	FString CurrentSessionName = TEXT("");
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