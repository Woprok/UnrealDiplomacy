// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionManager.generated.h"

// Forward Declarations

struct FUDActionPresentation;
class IUDActionInterface;
class UUDWorldGenerator;
class UUDModifierManager;

/**
 * Manages all actions that are allowed to execute.
 * Each action must be registered with this manager otherwise
 * it will not be executable.
 * 
 * Provides basic filters to retrieve information about actions.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDActionManager : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Returns Action associated with the supplied Id.
	 */
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IUDActionInterface> GetAction(int32 actionId);
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
#pragma region Action Filter API
public:
	/**
	 * Retrieves all actions that are tagged as stratagems.
	 */
	TArray<FUDActionPresentation> FilterStratagems();
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
	 * Tags are not changing during the game and this serves as filter startpoint.
	 */
	UPROPERTY()
	TArray<FUDActionPresentation> FilterStartpoint = { };
#pragma endregion
protected:
	/**
	 * Registers all core actions.
	 */
	UFUNCTION()
	void RegisterCoreActions();
	/**
	 * Registers all additional actions.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void RegisterAdditionalActions();
	/**
	 * Default method for registering multiple actions at once.
	 */
	UFUNCTION()
	void RegisterActionList(TArray<TScriptInterface<IUDActionInterface>> actionList);
	/**
	 * Binds all shared objects to an action that is being registered.
	 */
	UFUNCTION()
	void BindSharedToAction(TScriptInterface<IUDActionInterface> newAction);
	/**
	 * All actions registered with the manager.
	 */
	UPROPERTY()
	TMap<int32, TScriptInterface<IUDActionInterface>> Actions;
	/**
	 * World Generator shared by actions.
	 */
	UPROPERTY()
	TObjectPtr<UUDWorldGenerator> WorldGenerator = nullptr;
	/**
	 * Modifier Manager shared by actions.
	 */
	UPROPERTY()
	TObjectPtr<UUDModifierManager> ModifierManager = nullptr;
private:
	/**
	 * Determines if this was initialized and can be used.
	 */
	UPROPERTY()
	bool IsInitialized = false;
};