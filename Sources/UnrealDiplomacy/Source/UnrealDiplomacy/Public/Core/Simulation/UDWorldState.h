// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"

#include "UDModifierData.h"
#include "UDActionData.h"

#include "UDWorldState.generated.h"

// Forward Declarations

//struct FUDActionData;

/**
 * Represents single tile.
 * This needs to be duplicated as it's not generated by pure actions.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDTileState : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates new instance of the tile state for specific location in array.
	 */
	static TObjectPtr<UUDTileState> CreateState(int32 x, int32 y);
	/**
	 * Creates new instance of the tile state for specific location in array.
	 */
	static TObjectPtr<UUDTileState> Duplicate(TObjectPtr<UUDTileState> existingState);
public:
	/**
	 * Position in map array.
	 */
	UPROPERTY()
	FIntPoint Position;
	/**
	 * Owner's unique id.
	 * Default is UUDWorldState::GaiaWorldStateId as this is inherently part of world state.
	 */
	UPROPERTY()
	int32 OwnerUniqueId;
	/**
	 * Type associated with this tile.
	 * Used by generator and UI to properly convey information (requires proper tile mapping).
	 * TODO proper tile mapping, see pseudo implementation in UDSqureGrid.
	 * Useable by Modifiers etc...
	 * Same as ResourceType in current version.
	 */
	UPROPERTY()
	int32 Type;
	/**
	 * List of all modifiers applied to this tile.
	 */
	UPROPERTY()
	TArray<FUDModifierData> Modifiers;
	UPROPERTY()
	FString Name;
	UPROPERTY()
	int32 ResourceType;
	UPROPERTY()
	int32 ResourceStockpile;
};

/**
 * Represents map composed from tiles. 
 * This needs to be duplicated as it's not generated by pure actions.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDMapState : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates new instance of the map state.
	 */
	static TObjectPtr<UUDMapState> CreateState(int32 mapSeed, int32 xSizeOfMap, int32 ySizeOfMap, int32 requiredFactionSpace);
	/**
	 * Creates new instance of the map state based on existing one.
	 * Used for copying map to a state.
	 */
	static TObjectPtr<UUDMapState> Duplicate(TObjectPtr<UUDMapState> existingState);
public:
	/**
	 * Returns tile that is represented by provided FIntPoint.
	 */
	TObjectPtr<UUDTileState> GetTile(FIntPoint tile)
	{
		return Tiles[MapSizeOfY * tile.X + tile.Y];
	}
public:
	/**
	 * Each tile present in the world.
	 * Due to UE, this is represented as 1D array in Row-Major order (x * y).
	 * Access is then x * desired[x] + desired[y].
	 */
	UPROPERTY()
	TArray<TObjectPtr<UUDTileState>> Tiles;
	/** Seed used by generator. */
	UPROPERTY()
	int32 MapSeed = 0;
	/** X size of Map (first in array). */
	UPROPERTY()
	int32 MapSizeOfX = 0;
	/** Y size of Map (second in array). */
	UPROPERTY()
	int32 MapSizeOfY = 0;
	/** Faction Count that is required by setup to be accomondable. */
	UPROPERTY()
	int32 RequiredFactionSpace = 0;
};

/**
 * Represents current controller of the faction.
 */
UENUM(BlueprintType)
enum class EUDFactionController : uint8
{
	/**
	 * Undefined controller.
	 */
	Error = 0,
	/**
	 * Server is in charge of this faction as it's current server player.
	 */
	Gaia = 1,
	/**
	 * Server is in charge of this faction due to being owned by Observer.
	 */
	Observer = 2,
	/**
	 * Human is in charge of this faction.
	 */
	Player = 3,
	/**
	 * AI is in charge of this faction.
	 */
	AI = 4,
};

/**
 * Represents current type of the decision.
 */
UENUM(BlueprintType)
enum class EUDDecisionType : uint8
{
	/**
	 * Undefined type.
	 */
	Error = 0,
	/**
	 * Gifts are resolveable without any confirmation.
	 * Accept is not required. Effect is applied immediately.
	 * Decline is not possible.
	 */
	Gift = 1,
	/**
	 * Offers require confirmation, such as accepting loan from other faction.
	 * Accept applies effect. Generally applying effect to the target.
	 * Decline has no consequences.
	 */
	Offer = 2,
	/**
	 * Requests is polite way of asking for something, such as Military Access from other faction.
	 * Accept applies effect. Generally providing something to invoker.
	 * Decline has no consequences.
	 */
	Request = 3,
	/**
	 * Demands are actions that have consequences, if they are not accepted.
	 * Result is either provided action or decline action.
	 * Accept applies effect. Generally providing something to invoker. Negative for target.
	 * Decline applies consequence effect. Negative to target.
	 */
	Demand = 4,
};

/** Represents current result of the decision.  */
UENUM(BlueprintType)
enum class EUDDecisionResult : uint8
{
	/** Waiting for resolution. */
	Pending = 0,
	/** Accepted by target faction. */
	Confirmed = 0,
	/** Rejected by target faction. */
	Declined = 0,
};

/**
 * Represents single action created by bilateral diplomacy actions.
 */
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDDecision
{
	GENERATED_BODY()
public:
	FUDDecision() {}
	FUDDecision(EUDDecisionType type, FUDActionData action)
		: Type(type), ConfirmAction(action), HasDecline(false), Result(EUDDecisionResult::Pending) {}
	/** 
	 * Demands require decline action.This can be supplied in backup parameters.
	 * Thus enabling for both actions to be sent in single action decision.
	 */
	FUDDecision(EUDDecisionType type, FUDActionData action, FUDActionData declineAction)
		: Type(type), ConfirmAction(action), DeclineAction(declineAction), 
		HasDecline(true), Result(EUDDecisionResult::Pending) {}

	UPROPERTY(BlueprintReadOnly)
	EUDDecisionType Type;
	UPROPERTY(BlueprintReadOnly)
	FUDActionData ConfirmAction;
	UPROPERTY(BlueprintReadOnly)
	FUDActionData DeclineAction;
	UPROPERTY(BlueprintReadOnly)
	bool HasDecline = false;
	UPROPERTY(BlueprintReadOnly)
	EUDDecisionResult Result = EUDDecisionResult::Pending;
public:
	/**
	 * Equality over all fields.
	 */
	inline bool IsValueEqual(const FUDDecision& rhs) const
	{
		return
			Type == rhs.Type &&
			HasDecline == rhs.HasDecline &&
			Result == rhs.Result &&
			ConfirmAction.IsValueEqual(rhs.ConfirmAction) &&
			DeclineAction.IsValueEqual(rhs.DeclineAction);
	}
};

/**
 * Faction is primary control unit of each player or AI.
 * World is considered faction as well: Gaia Faction.
 * TODO Factions can be combined to Nation.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDFactionState : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates new instance of the player state for specified player.
	 */
	static TObjectPtr<UUDFactionState> CreateState(int32 factionId);
public:
	/**
	 * List of unresolved decisions created by actions 
	 * that are pending for confirm/decline action from this player.
	 * key is actions SourceUniqueId / BySource
	 */
	UPROPERTY()
	TMap<int32, FUDDecision> PendingDecisions;
	/**
	 * List of resolved decisions created by confirm and decline actions. 
	 * key is actions SourceUniqueId / BySource
	 */
	UPROPERTY()
	TMap<int32, FUDDecision> ResolvedDecisions;
	/**
	 * Defines current policy used for demands in case of rejection.
	 */
	UPROPERTY()
	int32 DecisionDemandPolicy;
	/**
	 * List of all modifiers applied to this faction.
	 */
	UPROPERTY()
	TArray<FUDModifierData> Modifiers;
	/**
	 * Unique id assigned to the owner.
	 */
	UPROPERTY()
	int32 PlayerUniqueId;
	/**
	 * Resources used by the game.
	 * First is resource Id, second is current value.
	 * Resource ids is supposed to be provided by ResourceManager.
	 * If resource manager does not exists it's defined as macro constant in world state header file.
	 */
	UPROPERTY()
	TMap<int32, int32> Resources;
	/**
	 * List of all available stratagem options to use.
	 * On other words what kind of expensive actions can be used.
	 */
	UPROPERTY()
	TSet<int32> StratagemOptions = { };
	/**
	 * Current faction name that can be displayed.
	 */
	UPROPERTY()
	FString Name;
	/**
	 * Defines what kind of controller is responsible for this faction.
	 */
	UPROPERTY()
	EUDFactionController Controller;
};

/**
 * Represents current state of the deal. 
 */
UENUM(BlueprintType)
enum class EUDDealSimulationState : uint8
{
	/**
	 * Error state.
	 */
	Undefined = 0,
	/**
	 * Deal draft is being created.
	 */
	CreatingDraft = 1,
	/**
	 * Players are assembling to discusss deal draft.
	 */
	Assembling = 2,
	/**
	 * Players are able to add additional points to draft.
	 */
	ExtendingDraft = 3,
	/**
	 * Players can demand or request something for being favorable toward the points.
	 */
	DemandsAndRequests = 4,
	/**
	 * Players are trying to appease these that oppose the deal.
	 */
	Bidding = 5,
	/**
	 * Players select bids they accept in exchange for voting favorably or leave.
	 */
	FinalizingDraft = 6,
	/**
	 * Remaining players vote.
	 */
	Vote = 7,
	/**
	 * Resolution. 
	 */
	Resolution = 8,
	/**
	 * Result.
	 */
	Result = 9,
};

/**
 * Represents current result state of the deal.
 * Deal is either opened or closed. Close is oen of multiple different reasons.
 */
UENUM(BlueprintType)
enum class EUDDealSimulationResult : uint8
{
	/**
	 * Error state.
	 */
	Undefined = 0,
	/**
	 * Deal is being discussed.
	 */
	Opened = 1,
	/**
	 * All voted positively.
	 */
	Passed = 5,
	/**
	 * Someone voted against the final deal form.
	 */
	Vetoed = 6,
	/**
	 * All other players either did not join or joined and then left.
	 */
	Disassembled = 7,
	/**
	 * Deal was closed by moderator.
	 */
	Closed = 8,
};

/**
 * Represents type of discussion point in terms of politeness.
 */
UENUM(BlueprintType)
enum class EUDPointType : uint8
{
	/**
	 * Invalid state.
	 */
	Error = 0,
	/**
	 * Player expects fair deal.
	 */
	Proposal = 1,
	/**
	 * Player expects to benefit from it.
	 */
	Demand = 2,
	/**
	 * Player expects to pay for it.
	 */
	Offer = 3,
};

/**
 * Represents single discussion point, argument or bid.
 * Based on later decision this does no longer carry multiple invokers and targets.
 * Fate of multi invoker is left for questioning when alliances are introduced.
 * At the moment this is only 1:1, instead of the original N:M
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDiscussionItem : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates new instance of the deal state.
	 */
	static TObjectPtr<UUDDiscussionItem> CreateState(int32 ownerId);
	/**
	 * Who is responsible for editing this point.
	 */
	int32 EditorId;
	/**
	 * Consequencies represent direct answer to this item. 
	 * Result is tree-like structure between Point->Arguments->Bids.
	 */
	TArray<int32> Consequencies;
	/**
	 * Determines if this item is still used, or is just preserved for consistency of reverting actions.
	 */
	bool IsIgnored = false;
	/**
	 * Invoked action.
	 */
	int32 ActionId;
	/**
	 * Each item has a specific overarching behaviour attached to it.
	 * Valid options may wary between different levels.
	 */
	EUDPointType Type = EUDPointType::Error;
	/**
	 * Faction that will invoke this action..
	 */
	int32 Invoker;
	/**
	 * Same as FUDActionData definition.
	 */
	TArray<int32> ValueParameters;
	/**
	 * Same as FUDActionData definition.
	 */
	FString TextParameter;
};


/**
 * Represents discussion action selected outcome.
 */
UENUM(BlueprintType)
enum class EUDDealActionResult : uint8
{
	/**
	 * Invalid state.
	 */
	Error = 0,
	/**
	 * This conditions were not yet accepted.
	 */
	Unresolved = 1,
	/**
	* Player accepted conditions.
	*/
	Accepted = 2,
	/**
	 * Player denied conditions.
	 */
	Denied = 3,
	/**
	 * Player altered parameters and accepted altered version.
	 * TODO PROVIDE UI SUPPORT FOR THIS.
	 */
	Changed = 4,
	/**
	 * Other player used sabotage.
	 * UNUSED. This state would require Sabotage action to be queued.
	 */
	//Sabotaged = 5,
};

/**
 * Represents single action created from discussion item.
 */
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDDiscussionAction
{
	GENERATED_BODY()
public:
	FUDDiscussionAction() {}
	FUDDiscussionAction(FUDActionData action, EUDDealActionResult result, bool isSabotaged, int32 sabotageId) 
		: Action(action), SelectedResult(result), WasSabotaged(isSabotaged), SabotageId(sabotageId) {}
	UPROPERTY(BlueprintReadOnly)
	FUDActionData Action;
	UPROPERTY(BlueprintReadOnly)
	EUDDealActionResult SelectedResult;
	/**
	 * This has to be outside of Result, even if it is result.
	 * We are still required to track original player action.
	 */
	UPROPERTY(BlueprintReadOnly)
	bool WasSabotaged;
	UPROPERTY(BlueprintReadOnly)
	int32 SabotageId;
};

/**
 * Represents single message.
 */
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDChatMessage
{
	GENERATED_BODY()
public:
	FUDChatMessage() {}
	FUDChatMessage(int32 authorId, FString message)
		: AuthorId(authorId), Message(message) {}
	UPROPERTY(BlueprintReadOnly)
	int32 AuthorId;
	UPROPERTY(BlueprintReadOnly)
	FString Message;
};

/**
 * Represents state of single deal that is being / was made.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealState : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates new instance of the deal state.
	 */
	static TObjectPtr<UUDDealState> CreateState(int32 dealId, int32 ownerId);
	/**
	 * Current state of the deal process.
	 */
	UPROPERTY()
	EUDDealSimulationState DealSimulationState = EUDDealSimulationState::Undefined;
	/**
	 * Current result of the deal process.
	 */
	UPROPERTY()
	EUDDealSimulationResult DealSimulationResult = EUDDealSimulationResult::Undefined;
	/**
	 * List of players that is currently participating in the deal.
	 */
	UPROPERTY()
	TArray<int32> Participants;
	/**
	 * List of players that can't be invited back to deal.
	 */
	UPROPERTY()
	TArray<int32> BlockedParticipants;
	/**
	 * Deal is identifiably by this Id. This Id is same as the Action Id that created this deal.  
	 */
	UPROPERTY()
	int32 UniqueDealId;
	/**
	 * Id of a Player that is moderating the deal.
	 */
	UPROPERTY()
	int32 OwnerUniqueId;
	/**
	 * List of all discussion points.
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UUDDiscussionItem>> Points;
	/**
	 * List of all primary points. This are the starting points for further evaluation and filters.
	 */
	UPROPERTY()
	TArray<int32> PrimaryPoints;
	/**
	 * Server received history of chat messages.
	 */
	UPROPERTY()
	TArray<FUDChatMessage> ChatHistory;
	/**
	 * List of players that selected ready in current phase.
	 * This will/should be automatically reset on phase change.
	 * This has no gameplay indication and is purely cosmetic.
	 * Refer to timer, once it's added as gameplay indicator.
	 */
	UPROPERTY()
	TArray<int32> IsReadyPlayerList;
	/**
	 * List of players that selected yes in the vote phase.
	 * This will be automatically resent on phase change.
	 */
	UPROPERTY()
	TArray<int32> PositiveVotePlayerList;
	/**
	 * All actions that are required to be executed and their current state.
	 * This is filled by an action and then updated by invoker for each action.
	 * In some cases it can be flagged as sabotaged.
	 * Actions are executed once all players determined their actions.
	 */
	UPROPERTY()
	TArray<FUDDiscussionAction> DealActionList;
	/**
	 * Current deal name that can be displayed.
	 */
	UPROPERTY()
	FString Name;
};

/**
 * Stores all information related to victory and loss conditions.
 */
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDThroneState
{
	GENERATED_BODY()
public:
	FUDThroneState() {}
	FUDThroneState(int32 currentRuler) : Ruler(currentRuler) {}
	/**
	 * Current Usurper. 0 is invalid/empty. Generally invalid when owned by World.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 Ruler = 0;
};

/**
 * Stores all information related to turns.
 * Turns are basically phases/acts. 
 * In each turn all players can act.
 * One player is selected regent, giving his faction more power during the turn.
 * Once all players choosed to finish turn, new regent is selected and next act will began.
 */
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDTurnState
{
	GENERATED_BODY()
public:
	FUDTurnState() {}
	 /**
	  * Turn order of all Factions, represented by their FactionId.
	  */
	UPROPERTY()
	TArray<int32> FactionTurnOrder = { };
	/**
	 * Current Faction that is playing.
	 * Default value is 0.
	 */
	UPROPERTY()
	int32 RegentFaction = 0;
	/**
	 * Current turn, has no purpose other than flavor info.
	 * Default value is 0.
	 */
	UPROPERTY()
	int32 Turn = 0;
	/**
	 * Factions that finished current turn.
	 */
	UPROPERTY()
	TSet<int32> TurnFinishedFactions = { };
	/**
	 * By default game is inbetween turns, so this is true.
	 * If this is false players are allowed to act.
	 */
	UPROPERTY()
	bool IsIntermezzo = true;
};

/**
 * Stores all information related to victory and loss conditions.
 */
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDSettings
{
	GENERATED_BODY()
public:
	FUDSettings() {}
	/** 
	 * AI player count that will be add to game. 
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 AICount = 0;
	/** 
	 * Seed used for generation. 
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 MapSeed = 0;
	/** 
	 * Width of the final map. 
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 MapWidth = 0;
	/** 
	 * Height of the final map. 
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 MapHeight = 0;
	/** 
	 * Amount of points available for use. 
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 StratagemPoints = 0;
};

/**
 * Explains the current state of world in relation to state of play.
 */
UENUM(BlueprintType)
enum class EUDWorldSimulationState : uint8
{
	/**
	 * World is being prepared for play.
	 * System,Gaia
	 */
	Preparing,
	/**
	 * Players action are allowed.
	 * System,Gaia,Game
	 */
	Simulating,
	/**
	 * Players action are no longer be allowed.
	 * System
	 */
	Finished,
	MAX UMETA(Hidden)
};

/**
 * Defines how is world state supposed to be seen.
 */
UENUM(BlueprintType)
enum class EUDWorldPerspective : uint8
{
	/**
	 * Gaia faction can perceive everything. 
	 */
	Gaia = 0,
	/**
	 * Observer is faction neutral and can perceive everything.
	 */
	Observer = 1,
	/**
	 * Rulers of the nation can perceive everything that is happening inside of their nation.
	 */
	Nation = 2,
	/**
	 * Factions are parts of nation and can perceive only their domain and sphere of influence.
	 */
	Faction = 3
};

/**
 * World state represents whole world as seen by certain Faction.
 * Gaia Faction always has access to full information.
 * Player & AI factions should have access to the parts they know about.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDWorldState : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates new instance of the world state.
	 * IsPlayerPerspectiveOnly defines if this state is supposed to hold all knowledge about the world.
	 */
	static TObjectPtr<UUDWorldState> CreateState(int32 playerId, EUDWorldPerspective perspectiveType);
public:
	// State Informations
	/**
	 * Owner of this state. Specific faction from Factions table.
	 */
	UPROPERTY()
	int32 FactionPerspective;
	/**
	 * Defines what can this state see.
	 */
	UPROPERTY()
	EUDWorldPerspective Perspective = EUDWorldPerspective::Faction;
	/**
	 * Describes what is the state based on simulation changes.
	 * State is not completely safe to read if it's not yet in PLAYING.
	 */
	UPROPERTY()
	EUDWorldSimulationState WorldSimulationState = EUDWorldSimulationState::Preparing;
public:
	// State Data
	/**
	 * Current Map of tiles.
	 */
	UPROPERTY()
	TObjectPtr<UUDMapState> Map;
	/**
	 * Map of players with key as their id.
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UUDFactionState>> Factions;
	/**
	 * All deals that are / were done during the game.
	 * key is Source / BySource
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UUDDealState>> Deals;
	/**
	 * Contains all necessary data for turn manipulation.
	 */
	UPROPERTY()
	FUDTurnState TurnData;
	/**
	 * Represents current state of imperial throne. 
	 * Winnerr is determined by the owner of the throne at the end of the game.
	 * If there is no throne owner player legitimacy will be acknowledged.
	 */
	UPROPERTY()
	FUDThroneState ImperialThrone;
	/**
	 * Default or current setting override.
	 */
	UPROPERTY()
	FUDSettings Settings;
};