// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionManager.generated.h"

// Forward Declarations

class IUDActionInterface;
class UUDWorldGenerator;
class UUDModifierManager;

/**
 * Manages all actions that are allowed to execute.
 * Each action must be registered with this manager otherwise
 * it will not be executable.
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
protected:
	/**
	 * Registers all core actions.
	 */
	UFUNCTION()
	void RegisterCoreActions();
	/**
	 * Registers all additional actions.
	 */
	UFUNCTION()
	virtual void RegisterDefaultActions();
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
	 * Determines if this was initialized and can be used;
	 */
	UPROPERTY()
	bool IsInitialized = false;
};