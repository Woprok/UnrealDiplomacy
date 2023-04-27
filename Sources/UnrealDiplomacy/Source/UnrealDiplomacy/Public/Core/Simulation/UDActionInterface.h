// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionData.h"
#include "UDWorldState.h"
#include "UDWorldGenerator.h"
#include "UDModifier.h"
#include "UDModifierManager.h"
#include "UDActionInterface.generated.h"

// TODO extend by adding more functions that do verify & check restriction

/** 
 * Specifier for interface used by IUDAction.
 * This is required by UE compiler. This can't contain UNREALDIPLOMACY_API macro.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UUDActionInterface : public UInterface
{
	GENERATED_BODY()
};

/** 
 * Interface for executable action.
 * Action is defined by ability to take data and apply their expected effect on simulation.
 */
class UNREALDIPLOMACY_API IUDActionInterface
{
	GENERATED_BODY()
public:
	/**
	 * Checks if this action is valid for current state.
	 * Validity is in general checked on GaiaWorldState.
	 */
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
	/**
	 * This applies action over the UUDWorldState.
	 */
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
	 /**
	  * If possible action should be able to revert, whatever kind of operation it did,
	  * if it's provided with the same data.
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