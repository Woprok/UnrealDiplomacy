// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UDGameInstance.generated.h"

// Forward Declarations

struct FUDApplicationSettings;

/**
 * Persistent state per game instance.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	/**
	 * Shortcut to retrieve casted GameInstance.
	 * Requries World pointer that can be retrieved by GetWorld().
	 */
	static TObjectPtr<UUDGameInstance> Get(TObjectPtr<UWorld> world);
	/**
	 * Loads current global application settings.
	 */
	FUDApplicationSettings LoadSettings();
	/**
	 * Applies changes to global application settings.
	 */
	void SaveSettings(FUDApplicationSettings newSettings);
	/**
	 * Invokes application exit.
	 */
	void Quit();
	/**
	 * Initializes new session.
	 */
	bool CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
private:
	TObjectPtr<UGameUserSettings> GetGameUserSettings();
};