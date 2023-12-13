// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionInterface.generated.h"

// Forward Declarations

struct FUDActionData;
struct FUDActionPresentation;
class UUDWorldState;
class UUDWorldGenerator;
class UUDWorldFactionGenerator;
class UUDModifierManager;
class UUDResourceManager;

// TODO Define proper order for parameters in ValueParameters ?
// SPECIALS:
// Map Settings
// TARGET -> TILE -> AMOUNT -> ACTION
// Amount
// Action
// Tile
// Tile Amount
// Tile Action
// Target
// Target Amount
// Target Tile
// -> Target Tile Amount
// -> Target Decision
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

// TODO Reevaluate if this tag system can be done it in different way to reduce complexity.

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
#define UD_ACTION_TAG_PARAMETER_DEAL 11
#define UD_ACTION_TAG_PARAMETER_FACTION 12
#define UD_ACTION_TAG_PARAMETER_TILE 13
#define UD_ACTION_TAG_PARAMETER_RESOURCE 14
#define UD_ACTION_TAG_PARAMETER_ACTION 15
#define UD_ACTION_TAG_PARAMETER_VALUE 16
#define UD_ACTION_TAG_PARAMETER_TEXT 17
// Details
#define UD_ACTION_TAG_PARAMETER_VALUE_AMOUNT 18
#define UD_ACTION_TAG_PARAMETER_VALUE_SMALL_MAX 19
// Decisions
// Direct action on target (Give Resource)
#define UD_ACTION_TAG_DECISION_DIRECT 20
// Offer action to target (Offer Alliance)
#define UD_ACTION_TAG_DECISION_OFFER 21
// Request action from target (Request Stratagem)
#define UD_ACTION_TAG_DECISION_REQUEST 22
// Demand action from target (Transfer Tile)
#define UD_ACTION_TAG_DECISION_DEMAND 23
// Defines action that can be executed on demand rejection.
#define UD_ACTION_TAG_DECISION_CONSEQUENCE 24

// Specifies for this action to have at least default resource cost
#define UD_ACTION_TAG_RESOURCE_COST 30

// This action is hidden from all UI, even if it would be normally visible.
//#define UD_ACTION_TAG_HIDDEN 30

// This is default and is not necessary.
#define UD_ACTION_TAG_VERIFY_PARAMETER_EQUALITY 50
// This overrides default check inside of an interface.
#define UD_ACTION_TAG_VERIFY_PARAMETER_MINIMUM 51

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
#define UD_PARAMETER_ARG_DEAL TEXT("DEAL")

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
	int32 ActionId = -1;
	/** List of all tags that are used to define behaviour in UI, etc... */
	TSet<int32> Tags;
	/** Main name of this action. */
	FString Name = TEXT("");
	/** 
	 * Defines format for converting action to offer message string based on action parameters. 
	 * To use this use format and supply all parameters required by tags.
	 */
	FString MessageContentFormat = TEXT("");
	/** 
	 * Defines format for converting action to deal string based on action parameters. 
	 * To use this use format and supply all parameters required by tags.
	 */
	FString DealContentFormat = TEXT("");
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
	/** Shared world generator that can be used by action. */
	virtual void SetWorldGenerator(TWeakObjectPtr<UUDWorldGenerator> generator);
	/** Shared world faction generator that can be used by action. */
	virtual void SetWorldFactionGenerator(TWeakObjectPtr<UUDWorldFactionGenerator> generator);
	/** Shared modifier manager that can be used by action. */
	virtual void SetModifierManager(TWeakObjectPtr<UUDModifierManager> manager);
	/** Shared resource manager that can be used by action. */
	virtual void SetResourceManager(TWeakObjectPtr<UUDResourceManager> manager);
	/**
	 * Formatted version of the action.
	 */
	virtual FString ToString() const;
	/**
	 * Contains definition for filtering and displaying action.
	 */
	virtual FUDActionPresentation GetPresentation() const;
	/** 
	 * Should this action bypass Stratagem Checks. 
	 * Exclusive for Deal Execution and any other action that wants to execute action on behalf of the faction.
	 * This means that action will be executed even if faction already used it (from the last reset).
	 * And that action can be used, even if invoker does not have a stratagem.
	 * Note that decisions use continuations as well, but these are not supposed to bypass stratagems.
	 */
	virtual bool IsBypassingStratagemRequirements() const;
};