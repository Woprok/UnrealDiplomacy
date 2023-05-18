// Copyright Miroslav Valach
// Based on as there is nothing new to invent https://cedric-neukirchen.net/docs/session-management/sessions-in-cpp
// https://unreal.gg-labs.com/v/4.25/wiki-archives/networking/how-to-use-sessions-in-c++
// Blueprints were easier to setup, but due to almost everything being c++, this was changed as well.

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
	 * Registers delegates for future use.
	 */
	UUDSessionSubsystem();
	/**
	 * Defines Session Paramters that will be used for next session creation.
	 */
	void CreateSettings(FString SettingLevelName, int32 NumPublicConnections = 8, bool IsLANMatch = true);
	/**
	 * Defines Session Parameters that will be used for next session update.
	 */
	void UpdateSettings(FString SettingLevelName);
	/**
	 * Defines Session Search Paramters that will be used for next session search.
	 */
	void CreateSearchSettings(int32 MaxSearchResults = 20, bool IsLANQuery = true);
	/**
	 * Creates new session with specified name and settings specified by last CreateSettings.
	 */
	void CreateSession(FName SessionName);
	/**
	 * Updates session with specified name and settings specified by last UpdateSettings.
	 */
	void UpdateSession(FName SessionName);
	/**
	 * Starts session with specified name.
	 * After this it's expected to travel to another level.
	 */
	void StartSession(FName SessionName);
	/**
	 * Ends session with specified name.
	 * In most cases session is outright destroyed rather then ended.
	 */
	void EndSession(FName SessionName);
	/**
	 * Destroys session with specified name.
	 * Both client and server have to destroy session before creating or joining next.
	 */
	void DestroySession(FName SessionName);
	/**
	 * Find active sessions.
	 */
	void FindSessions();
	/**
	 * Join to specified session that was found by FindSessions.
	 */
	void JoinGameSession(FName SessionName, const FOnlineSessionSearchResult& SessionResult);
	/**
	 * Provides default session name from OnlineSessionNames::NAME_GameSession.
	 */
	FName GetDefaultSessionName();
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
	 * Example of travel, if the start succeeded.
	 */
	void OnStartSuccessTravel()
	{
		// If the start was successful, we can open a NewMap if we want. Make sure to
		// use "listen" as a parameter!
		//UGameplayStatics::OpenLevel(GetWorld(), "NewMap", true, "listen");
	}
	/**
	 * Example of travel, if the destroyed succeeded.
	 */
	void OnDestroySuccessTravel()
	{
		//UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap", true);
	}
	/**
	 * Example of travel, if the join succeeded.
	 */
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
	/**
	 * SEE ON_JOIN_SUCCESS_TRAVEL THIS IS SAME THING
	 */
	bool TryTravelToCurrentSession(FName SessionName);
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
	// Internal Functions for Handles
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnEndSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
private:
	FString SETTING_LEVELNAME = TEXT("Skirmish");
	// Settings
	TSharedPtr<FOnlineSessionSettings> CurrentSessionSettings;
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