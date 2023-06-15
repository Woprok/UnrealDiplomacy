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

// TODO Define proper order for parameters in ValueParameters ?
// SPECIALS:
// Map Settings
// TARGET/TILE/AMOUNT
// Amount
// Tile
// Tile Amount
// Target
// Target Amount
// Target Tile
// -> Target Tile Amount
// DEAL -> POINT OR ELSE -> POINT MODIFICATION
// Deal
// Deal Point
// Deal Point Target
// Deal Point Value
// Deal Point Tile
// Deal Point Tile Value
// Deal Target
// Deal ConstructPoint
// SETTINGS
// Value

// TODO Reevaluate if this can be done it in different way to reduce complexity.

// Useable for internal purposes only.
#define UD_ACTION_TAG_INVALID 0
// Useable for external purposes as well.
#define UD_ACTION_TAG_VALID 1
// Can be selected as stratagem and costs stratagem points.
#define UD_ACTION_TAG_STRATAGEM 2
// Specific cost for a stratagem to be selected.
#define UD_ACTION_TAG_STRATAGEM_COST_1 3
#define UD_ACTION_TAG_STRATAGEM_COST_2 4
#define UD_ACTION_TAG_STRATAGEM_COST_3 5
#define UD_ACTION_TAG_STRATAGEM_COST_4 6
#define UD_ACTION_TAG_STRATAGEM_COST_5 7
// Faction can use it as interaction with other faction.
#define UD_ACTION_TAG_FACTION_INTERACTION 8

/**
 * Defines informations about the action and how it can be displayed in UI.
 */
USTRUCT()
struct FUDActionPresentation
{
	GENERATED_BODY()
public:
	FUDActionPresentation() {}
	/** Unique Action Id. */
	int32 ActionId;
	/** List of all tags that are used to define behaviour in UI, etc... */
	TSet<int32> Tags;
	/** Main name of this action. */
	FString Name = TEXT("");
};

/** 
 * Stumb for Action Interface.
 * Unreal Engine requires Interface to be first defined as empty stumb.
 * https://docs.unrealengine.com/5.2/en-US/interfaces-in-unreal-engine/
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
	virtual TArray<FUDActionData> GetContinuations(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const;
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
protected:
	/**
	 * Adds pending request associated with action and specified target.
	 */
	void AddPendingTargetRequest(FUDActionData action, int32 targetId, TObjectPtr<UUDWorldState> world);
	/**
	 * Removes pending request associated with action and specified target.
	 */
	void RemovePendingTargetRequest(const FUDActionData& action, int32 targetId, TObjectPtr<UUDWorldState> world);
	/**
	 * Checks if there is pending request associated with action and specified target.
	 */
	bool IsPendingTargetRequest(const FUDActionData& action, int32 targetId, TObjectPtr<UUDWorldState> world) const;
	/**
	 * Checks if there is pending interchangeable request associated with target.
	 */
	bool IsPendingInterchangeableTargetRequest(const FUDActionData& action, int32 targetId, TObjectPtr<UUDWorldState> world) const;
};