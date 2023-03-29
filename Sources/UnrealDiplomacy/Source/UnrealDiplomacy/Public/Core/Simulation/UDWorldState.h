// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionData.h"
#include "UDModifier.h"
#include "UDWorldState.generated.h"

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
	 */
	UPROPERTY()
	int32 Type;
	/**
	 * List of all modifiers applied to this tile.
	 * This are modifiers that are applied to this tile.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UUDModifier>> Modifiers;
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
	static TObjectPtr<UUDMapState> CreateState(int32 mapSeed, int32 xSizeOfMap, int32 ySizeOfMap);
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
		return Tiles[MapSizeOfX * tile.X + tile.Y];
	}
public:
	/**
	 * Each tile present in the world.
	 * Due to UE, this is represented as 1D array in Row-Major order (x * y).
	 * Access is then x * desired[x] + desired[y].
	 */
	UPROPERTY()
	TArray<TObjectPtr<UUDTileState>> Tiles;
	/**
	 * Seed used by generator.
	 */
	UPROPERTY()
	int32 MapSeed = 0;
	/**
	 * X size of Map (first in array).
	 */
	UPROPERTY()
	int32 MapSizeOfX = 0;
	/**
	 * Y size of Map (second in array).
	 */
	UPROPERTY()
	int32 MapSizeOfY = 0;
};

/**
 * Represents single Player/Ai state.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDNationState : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates new instance of the player state for specified player.
	 */
	static TObjectPtr<UUDNationState> CreateState(int32 playerId);
public:
	/**
	 * List of unresolved requests created by actions 
	 * that are pending for confirm/reject action from this player.
	 * key is Source / BySource
	 */
	UPROPERTY()
	TMap<int32, FUDActionData> PendingRequests;
	/**
	 * List of all modifiers applied to this player.
	 * This are modifiers that are applied to this player.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UUDModifier>> Modifiers;
	/**
	 * Unique id assigned to the owner.
	 */
	UPROPERTY()
	int32 PlayerUniqueId;
	/**
	 *
	 */
	UPROPERTY()
	int32 ResourceGold = 0;
};

/**
 * Explains the current state of world in relation to state of play.
 */
UENUM(BlueprintType)
enum class EUDWorldSimulationState : uint8
{
	/**
	 * World is being prepared for play.
	 */
	INITIALIZING,
	/**
	 * Players action are allowed.
	 */
	PLAYING,
	/**
	 * Players action are no longer be allowed.
	 */
	FINISHING,
	MAX UMETA(Hidden)
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
	  * Players that are affected by this action, this covers primarily case such as alliance.
	  * In case of alliance it makes no sense to distinguish between them.
	  */
	 TArray<int32> Participants;
	 /**
	  * Players that are invoking by action, so these are on left side of an action.
	  */
	 TArray<int32> Invokers;
	 /**
	  * Players that are targeted by action, so these are on right side of an action.
	  */
	 TArray<int32> Targets;
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
	TArray<FString> ChatHistory;
	/**
	 * List of players that selected ready in current phase.
	 * This will be automatically resent on phase change.
	 */
	UPROPERTY()
	TArray<int32> IsReadyPlayerList;
};

/**
 * Holds the state of a world.
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
	static TObjectPtr<UUDWorldState> CreateState(int32 playerId, bool isPlayerPerspectiveOnly);
	/**
	 * Gaia id.
	 */
	static const int32 GaiaWorldStateId = 0;
public:
	/**
	 * Current Map of tiles.
	 */
	UPROPERTY()
	TObjectPtr<UUDMapState> Map;
	/**
	 * Map of players with key as their id.
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UUDNationState>> Players;
	/**
	 * List of players in turn order, represented only by their unique id.
	 */
	UPROPERTY()
	TArray<int32> PlayerOrder;
	/**
	 * Id associated with a Player/Ai, that controls this simulation.
	 */
	UPROPERTY()
	int32 PerspectivePlayerId;
	/**
	 * State either belongs to specific Player/Ai or is global.
	 */
	UPROPERTY()
	bool IsPlayerPerspectiveOnly;
	/**
	 * Current Player/Ai/Server that is able to act.
	 * Default value is 0.
	 */
	UPROPERTY()
	int32 CurrentTurnPlayerId = 0;
	/**
	 * Current turn, has no purpose other than flavor info.
	 * Default value is 0.
	 */
	UPROPERTY()
	int32 CurrentTurn = 0;
	/**
	 * Describes what is the state based on simulation changes.
	 * State is not completely safe to read if it's not yet in PLAYING.
	 */
	UPROPERTY()
	EUDWorldSimulationState WorldSimulationState = EUDWorldSimulationState::INITIALIZING;
	/**
	 * All deals that are / were done during the game.
	 * key is Source / BySource
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UUDDealState>> Deals;
};