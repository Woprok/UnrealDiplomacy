// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionManager.generated.h"

// Forward Declarations

struct FUDActionPresentation;
class IUDActionInterface;
class UUDWorldGenerator;
class UUDWorldFactionGenerator;
class UUDModifierManager;
class UUDResourceManager;
class UUDStratagemOperationManager;

/**
 * Manages all actions that are allowed to execute.
 * Each action must be registered with this manager otherwise
 * it will not be executable.
 * 
 * Provides basic filters to retrieve information about actions.
 * This is stateless and only keeps references to his childs, that are as well stateless.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDActionManager : public UObject
{
	GENERATED_BODY()
public:
	/** Sets inner instance of modifier manager */
	void SetModifierManager(TWeakObjectPtr<UUDModifierManager> manager);
	/** Sets inner instance of resource manager */
	void SetResourceManager(TWeakObjectPtr<UUDResourceManager> manager);
	/** Sets inner instance of stratagem use manager */
	void SetStratagemOperationManager(TWeakObjectPtr<UUDStratagemOperationManager> manager);
	/** Retrieves inner instance of modifier manager */
	TWeakObjectPtr<UUDModifierManager> GetModifierManager();
	/** Retrieves inner instance of resource manager */
	TWeakObjectPtr<UUDResourceManager> GetResourceManager();
	/** Retrieves inner instance of stratagem use manager */
	TWeakObjectPtr<UUDStratagemOperationManager> GetStratagemOperationManager();
public:
	/**
	 * Returns Action associated with the supplied Id.
	 */
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IUDActionInterface> GetAction(int32 actionTypeId);
	/**
	 * Returns true if action was registered succeffully.
	 * This can fail if action has duplicate id with already existing one.
	 */
	UFUNCTION(BlueprintCallable)
	bool RegisterAction(TScriptInterface<IUDActionInterface> newAction);
	/**
	 * Safely initializes all dependencies.
	 */
	UFUNCTION(BlueprintCallable)
	void Initialize();
#pragma region Action Type Filter API
public:
	/**
	 * Retrieves all actions that are tagged as stratagems.
	 */
	TArray<FUDActionPresentation> FilterStratagems();
	/**
	 * Retrieves all actions that are tagged with one specific tag.
	 */
	TArray<FUDActionPresentation> FilterActionsWithTag(int32 searchedTag);
	/**
	 * Retrieves all actions that are tagged as faction interactions.
	 * Faction interaction is any action that targets directly other faction.
	 */
	TArray<FUDActionPresentation> FilterFactionInteractions(int32 additionalTag);
	/**
	 * Retrieves all actions that are tagged as tile interactions.
	 * Tile interaction is any action that targets tile directly.
	 */
	TArray<FUDActionPresentation> FilterTileInteractions();
	/**
	 * Retrieves specifeid action presentation.
	 */
	FUDActionPresentation GetSpecified(int32 actionId);
private:
	/**
	 * Retrieves all actions that are tagged by specified tag.
	 */
	TArray<FUDActionPresentation> FilterByTag(const TArray<FUDActionPresentation>& selection, int32 tag);
	/**
	 * Retrieves all actions that are tagged by specified tags.
	 */
	TArray<FUDActionPresentation> FilterByTags(const TArray<FUDActionPresentation>& selection, TSet<int32> tags);
	/**
	 * Tags are not changing during the game and this serves as filter startpoint.
	 */
	UPROPERTY()
	TArray<FUDActionPresentation> FilterStartpoint = { };
#pragma endregion
protected:
	/** Registers all core actions. */
	UFUNCTION()
	void RegisterCoreActions();
	/** Registers all blueprint actions. This takes precedence over the core. */
	virtual void RegisterBlueprintActions();
	/** Default method for registering multiple actions at once. */
	UFUNCTION()
	void RegisterActionList(TArray<TScriptInterface<IUDActionInterface>> actionList);
	/** Binds all shared objects to an action that is being registered.  */
	UFUNCTION()
	void BindSharedToAction(TScriptInterface<IUDActionInterface> newAction);
	/** All actions registered with the manager. */
	UPROPERTY()
	TMap<int32, TScriptInterface<IUDActionInterface>> Actions;
	/** World Generator shared by actions. */
	UPROPERTY()
	TObjectPtr<UUDWorldGenerator> WorldGenerator = nullptr;
	/** World Faction Generator shared by actions. */
	UPROPERTY()
	TObjectPtr<UUDWorldFactionGenerator> WorldFactionGenerator = nullptr;
	/** Modifier Manager shared by actions. */
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
	/** Resource Manager shared by actions. */
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;
	/** StratagemUse Manager shared by actions. */
	UPROPERTY()
	TWeakObjectPtr<UUDStratagemOperationManager> StratagemOperationManager = nullptr;
	/** Enables blueprint defined and overrides for registration. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	TArray<TSubclassOf<UObject>> BlueprintActions;
private:
	/**
	 * Determines if this was initialized and can be used.
	 */
	UPROPERTY()
	bool IsInitialized = false;
};