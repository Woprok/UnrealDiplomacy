// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionInterface.generated.h"

// Forward Declarations

struct FUDActionData;
class UUDWorldState;
class UUDWorldGenerator;
class UUDModifierManager;

/** 
 * Stumb for Action Interface.
 * Unreal Engine requires Interface to be first defined as empty stumb.
 * https://docs.unrealengine.com/5.1/en-US/interfaces-in-unreal-engine/
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UUDActionInterface : public UInterface
{
	GENERATED_BODY()
};

/** 
 * Actions are executable operations that can apply themselves on simulated state based on parameters.
 * Actions must be able to revert any effect it applied on a state.
 */
class UNREALDIPLOMACY_API IUDActionInterface
{
	GENERATED_BODY()
public:
	/**
	 * Evaluates if the action is executable on the provided state.
	 */
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
	/**
	 * Applies action over the provided state.
	 */
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
	/**
	 * Applies inverse of the Execute on the provided state.
	 */
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
	/**
	 * Unique action id that is provided for each action.
	 * This is directly referenced by FUDActionData to determine,
	 * who can execute it as the FUDActionData is very simple container.
	 * This has to be overriden, otherwise the action will be discarded (valid values are 0 - int32.MAX).
	 */
	virtual int32 GetActionTypeId()
	{
		// Default Interface call returns invalid value, e.g. -1.
		return -1;
	}
	virtual int32 GetRequiredParametersCount()
	{
		// Default Interface call returns invalid value, e.g. -1.
		return -1;
	}
	/**
	 * Set the instance of world generator used by all registred actions.
	 * If actions share world generator, they can use same data.
	 * Note: all actions that use generator needs this to be set.
	 * By default set this to every action.
	 */
	virtual void SetWorldGenerator(TObjectPtr<UUDWorldGenerator> worldGenerator)
	{
	 // Default Interface call is empty.
	}
	/**
	 * Set the instance of modifier manager used by all registred actions.
	 * Actions share modifier manager to correctly handle all modifiers.
	 * Note: all actions that use modifiers needs this to be set.
	 * By default set this to every action.
	 */
	virtual void SetModifierManager(TObjectPtr<UUDModifierManager> modifierManager)
	{
	 // Default Interface call is empty.
	}
	/**
	 * Determines if this action is eligible to execute additional actions.
	 * Additional actions should never be composite to prevent loop!
	 */
	virtual bool IsComposite()
	{
	 return false;
	}
	/**
	 * Additional actions that should be executed after this action, based on input.
	 * Validity is in general checked on GaiaWorldState.
	 */
	virtual TArray<FUDActionData> GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState);
	/**
	 * Indicates if this action requires additional backup before being applied.
	 */
	virtual bool RequiresBackup();
	/** 
	 * Backups specific data into provided action data so they can be restored on revert to original state.
	 */
	virtual void Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
};