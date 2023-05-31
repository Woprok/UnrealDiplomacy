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
	 * Travels to associated level used for menu / non game state.
	 */
	void OpenMenuLevel(FString options = "");
	/**
	 * Travels to associated level used for game.
	 */
	void OpenSkirmishLevel(FString options = "");
	/**
	 * Retrieves name associated with skirmish level used for game.
	 */
	FString GetSkirmishLevelName();
protected:
	/**
	 * Level that is used as menu and fallback.
	 */
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MenuLevel;
	/**
	 * Level that is used for skirmish game.
	 */
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> SkirmishLevel;
private:
	TObjectPtr<UGameUserSettings> GetGameUserSettings();
};