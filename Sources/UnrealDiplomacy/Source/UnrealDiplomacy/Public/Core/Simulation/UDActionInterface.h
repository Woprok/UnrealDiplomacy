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

// This is tag value that is never used for anything.
// Safe to be used as placeholder or value that is desired to be ignored.
#define UD_INVALID_TAG_ID -1
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
// Faction can use it as interaction with tile.
#define UD_ACTION_TAG_TILE_INTERACTION 9
// Specific parameter that should be included in parameter list.
#define UD_ACTION_TAG_PARAMETER_NONE 10
#define UD_ACTION_TAG_PARAMETER_FACTION 11
#define UD_ACTION_TAG_PARAMETER_TILE 12
#define UD_ACTION_TAG_PARAMETER_RESOURCE 13
#define UD_ACTION_TAG_PARAMETER_ACTION 14
#define UD_ACTION_TAG_PARAMETER_VALUE 15
#define UD_ACTION_TAG_PARAMETER_TEXT 16
// Details
#define UD_ACTION_TAG_PARAMETER_VALUE_AMOUNT 17
#define UD_ACTION_TAG_PARAMETER_VALUE_SMALL_MAX 18

// This is definitely start of global nightmare...
// Do not change the values and in case you have to change it inside of all actions.
// Outside of the actions everything refers to it with macro name...
#define UD_PARAMETER_ARG_FACTION_INVOKER TEXT("INVOKER")
#define UD_PARAMETER_ARG_FACTION_TARGET TEXT("TARGET")
#define UD_PARAMETER_ARG_TILE TEXT("TILE")
#define UD_PARAMETER_ARG_ACTION TEXT("ACTION")
#define UD_PARAMETER_ARG_RESOURCE TEXT("RESOURCE")
#define UD_PARAMETER_ARG_VALUE TEXT("VALUE")
#define UD_PARAMETER_ARG_TEXT TEXT("TEXT")

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
	/** Defines if this action is acceptable. If not returns InvalidActionId. */
	int32 AcceptActionId;
	/** Defines if this action is rejectable. If not returns InvalidActionId. */
	int32 RejectActionId;
	/** 
	 * Defines format for converting action to offer message string based on action parameters. 
	 * To use this use format and supply all parameters required by tags.
	 */
	FString MessageContentFormat = TEXT("");
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