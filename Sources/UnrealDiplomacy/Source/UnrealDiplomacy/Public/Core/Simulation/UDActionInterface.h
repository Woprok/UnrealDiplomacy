// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionInterface.generated.h"

// Forward Declarations

struct FUDActionData;
struct FUDActionPresentation;
class UUDWorldState;
class UUDWorldGenerator;
class UUDModifierManager;

/** 
 * Stumb for Action Interface.
 * Unreal Engine requires Interface to be first defined as empty stumb.
 * https://docs.unrealengine.com/5.1/en-US/interfaces-in-unreal-engine/
 */
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UUDActionInterface : public UInterface
{
	GENERATED_BODY()
};

/** 
 * Actions are executable operations that can apply themselves on simulated state based on parameters.
 * Actions are required to be able to revert any effect they applied on a state.
 */
class UNREALDIPLOMACY_API IUDActionInterface
{
	GENERATED_BODY()
public:
	/**
	 * Evaluates if the action is executable on the provided state.
	 */
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const;
	/**
	 * Applies action over the provided state.
	 */
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world);
	/**
	 * Applies inverse of the Execute on the provided state.
	 */
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world);
	/**
	 * Actions are identified by their unique Id.
	 * Valid values are 0 - int32.MAX.
	 * Only UUDSystemActionInvalid and interface are allowed to return -1;
	 */
	virtual int32 GetId() const;
	/**
	 * Additional parameters required by the action.
	 * Valid values are 0 - int32.MAX.
	 */
	virtual int32 GetParameterCount() const;
	/**
	 * Defines if action requires to be followed by other actions to complete execution.
	 */
	virtual bool HasContinuations() const;
	/**
	 * Additional actions that should be executed after this action, based on input.
	 * Validity is in general checked on GaiaWorldState.
	 */
	virtual TArray<FUDActionData> GetContinuations(const FUDActionData& action, TObjectPtr<UUDWorldState> world);
	/**
	 * Defines if action requires backup before being applied.
	 */
	virtual bool IsBackupRequired() const;
	/**
	 * Enables to backup additional data to the provided action action.
	 */
	virtual void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world);
	/**
	 * Shared world generator that can be used by action.
	 */
	virtual void SetWorldGenerator(TObjectPtr<UUDWorldGenerator> generator);
	/**
	 * Shared modifier manager that can be used by action.
	 */
	virtual void SetModifierManager(TObjectPtr<UUDModifierManager> manager);
	/**
	 * Formatted version of the action.
	 */
	virtual FString ToString() const;
	/**
	 * Contains definition for filtering and displaying action.
	 */
	virtual FUDActionPresentation GetPresentation() const;
};