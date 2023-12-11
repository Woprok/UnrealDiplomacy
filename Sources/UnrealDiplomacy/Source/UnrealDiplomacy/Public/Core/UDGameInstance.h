// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UDGameInstance.generated.h"

// Forward Declarations

struct FUDApplicationSettings;
class UUDActionManager;
class UUDModifierManager;
class UUDResourceManager;
class UUDSettingManager;
class UUDStratagemUseManager;

/**
 * Persistent state per game instance.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UUDGameInstance();
public:
	/**
	 * Shortcut to retrieve casted GameInstance.
	 * Requries World pointer that can be retrieved by GetWorld().
	 */
	static TObjectPtr<UUDGameInstance> Get(TObjectPtr<UWorld> world);
	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init() override;
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
public:
	/** Change this if you want to use BP implementation of manager. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
	TSubclassOf<UUDActionManager> ActionManagerType;
	/** Change this if you want to use BP implementation of manager. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
	TSubclassOf<UUDModifierManager> ModifierManagerType;
	/** Change this if you want to use BP implementation of manager. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
	TSubclassOf<UUDResourceManager> ResourceManagerType;
	/** Change this if you want to use BP implementation of manager. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
	TSubclassOf<UUDStratagemUseManager> StratagemUseManagerType;
	/** Change this if you want to use BP implementation of manager. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
	TSubclassOf<UUDSettingManager> SettingManagerType;
	/** Manager shared by application during the whole runtime. */
	TWeakObjectPtr<UUDActionManager> GetActionManager();
	/** Manager shared by application during the whole runtime. */
	TWeakObjectPtr<UUDModifierManager> GetModifierManager();
	/** Manager shared by application during the whole runtime. */
	TWeakObjectPtr<UUDResourceManager> GetResourceManager();
	/** Manager shared by application during the whole runtime. */
	TWeakObjectPtr<UUDStratagemUseManager> GetStratagemUseManager();
	/** Manager shared by application during the whole runtime. */
	TWeakObjectPtr<UUDSettingManager> GetSettingManager();
protected:
	/** Manager shared by application during the whole runtime. */
	UPROPERTY()
	TObjectPtr<UUDActionManager> ActionManager = nullptr;
	/** Manager shared by application during the whole runtime. */
	UPROPERTY()
	TObjectPtr<UUDModifierManager> ModifierManager = nullptr;
	/** Manager shared by application during the whole runtime. */
	UPROPERTY()
	TObjectPtr<UUDResourceManager> ResourceManager = nullptr;
	/** Manager shared by application during the whole runtime. */
	UPROPERTY()
	TObjectPtr<UUDStratagemUseManager> StratagemUseManager = nullptr;
	/** Manager shared by application during the whole runtime. */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UUDSettingManager> SettingManager = nullptr;
private:
	TObjectPtr<UGameUserSettings> GetGameUserSettings();
};