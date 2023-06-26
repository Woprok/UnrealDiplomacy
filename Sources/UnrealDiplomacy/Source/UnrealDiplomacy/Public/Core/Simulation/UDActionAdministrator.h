// Copyright Miroslav Valach
// TODO Transform ActionAdministrator into multiple models and approriate model manager class.

#pragma once

#include "CoreMinimal.h"
#include "UDActionInterface.h"
#include "UDWorldState.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"

// Forward Declarations

class IUDActionInterface;
class UUDWorldState;
class UUDActionManager;
class UUDActionDatabase;

enum class EUDGameStateInfo : uint8;
struct FUDGameOverInfo;
struct FUDStratagemPickableInfo;
struct FUDRegencyTurnInfo;
struct FUDThroneInfo;
struct FUDAvailableDiplomacyInfo;

struct FUDParameterListInfo;
struct FUDFactionInfo;
struct FUDFactionMinimalInfo;
struct FUDFactionInteractionInfo;
struct FUDTileInfo;
struct FUDTileMinimalInfo;
struct FUDTileInteractionInfo;
struct FUDResourceInfo;
struct FUDResourceMinimalInfo;
struct FUDActionMinimalInfo;

struct FUDModifierData;
struct FUDModifierInfo;

struct FUDMessageInfo;
struct FUDMessageInteractionInfo;

struct FUDDealParameter;
struct FUDFactionParameter;
struct FUDTileParameter;
struct FUDActionParameter;
struct FUDResourceParameter;
struct FUDValueParameter;
struct FUDTextParameter;
typedef TVariant<FUDDealParameter ,FUDFactionParameter, FUDTileParameter, FUDActionParameter, 
	FUDResourceParameter, FUDValueParameter, FUDTextParameter> ParameterData;

struct FUDDealInteractionInfo;
struct FUDChatMessageInfo;
struct FUDDealMinimalInfo;
struct FUDDealInfo;
struct FUDDealFactionInfo;
struct FUDDealPointMinimalInfo;

#include "Core/Simulation/Actions/UDDealActionContractCreate.h"

#include "UDActionAdministrator.generated.h"


USTRUCT(BlueprintType)
struct FUDPlayerInfo
{
	GENERATED_BODY()
public:
	FUDPlayerInfo() {}
	FUDPlayerInfo(int32 id) : Id(id) { }
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FText Name = FText::GetEmpty();
};

USTRUCT(BlueprintType)
struct FUDNationInfo
{
	GENERATED_BODY()
public:
	FUDNationInfo() {}
	FUDNationInfo(int32 id, int32 gold) : Id(id), Gold(gold) {}
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Gold = 0;
};

USTRUCT(BlueprintType)
struct FUDDealParticipantsInfo
{
	GENERATED_BODY()
public:
	FUDDealParticipantsInfo() {}
	FUDDealParticipantsInfo(int32 dealUniqueId, TArray<FUDPlayerInfo> active, TArray<FUDPlayerInfo> blocked, TArray<FUDPlayerInfo> available) 
		: DealUniqueId(dealUniqueId),
		ActiveParticipants(active), BlockedParticipants(blocked), AvailableParticipants(available) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDPlayerInfo> ActiveParticipants;
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDPlayerInfo> BlockedParticipants;
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDPlayerInfo> AvailableParticipants;
};

USTRUCT(BlueprintType)
struct FUDDealPointChildInfo
{
	GENERATED_BODY()
public:
	FUDDealPointChildInfo() {}
	FUDDealPointChildInfo(int32 dealUniqueId, int32 pointUniqueId)
		: DealUniqueId(dealUniqueId), PointUniqueId(pointUniqueId) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointUniqueId = 0;
};

USTRUCT(BlueprintType)
struct FUDDealPointTreeInfo
{
	GENERATED_BODY()
public:
	FUDDealPointTreeInfo() {}
	FUDDealPointTreeInfo(int32 dealUniqueId, int32 pointCount, TArray<FUDDealPointChildInfo> childPoints)
		: DealUniqueId(dealUniqueId), TotalPointCount(pointCount), PrimaryPoints(childPoints) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 TotalPointCount = 0;
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDDealPointChildInfo> PrimaryPoints;
};

USTRUCT(BlueprintType)
struct FUDDealPointInfo
{
	GENERATED_BODY()
public:
	FUDDealPointInfo() {}
	FUDDealPointInfo(int32 dealUniqueId, int32 pointUniqueId, EUDPointType type, int32 actionId) 
		: DealUniqueId(dealUniqueId), PointUniqueId(pointUniqueId), Type(type), ActionId(actionId) {}
	FUDDealPointInfo(int32 dealUniqueId, int32 pointUniqueId, EUDPointType type, int32 actionId,
		TArray<int32> valueParameters, FString textParameter)
		: DealUniqueId(dealUniqueId), PointUniqueId(pointUniqueId), Type(type), ActionId(actionId),
		ValueParameters(valueParameters), TextParameter(textParameter) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	EUDPointType Type;
	UPROPERTY(BlueprintReadOnly)
	int32 ActionId;
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> ValueParameters = {};
	UPROPERTY(BlueprintReadOnly)
	FString TextParameter;
};

USTRUCT(BlueprintType)
struct FUDDealActionInfo
{
	GENERATED_BODY()
public:
	FUDDealActionInfo() {}
	FUDDealActionInfo(int32 dealUniqueId, int32 actionIndex, FUDDiscussionAction actionBody)
		: DealUniqueId(dealUniqueId), ActionIndex(actionIndex), ActionBody(actionBody) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId;
	UPROPERTY(BlueprintReadOnly)
	int32 ActionIndex;
	UPROPERTY(BlueprintReadOnly)
	FUDDiscussionAction ActionBody;
};

#define LOCTEXT_NAMESPACE "ActionAdministrator"

DECLARE_DELEGATE_OneParam(UserActionRequestedDelegate, FUDActionData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDOnDataChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDOnDataReloaded);

/**
 * Model that is capable of converting internal state 
 * and represent it in more cohorent form.
 * At the 
 * Allows Player to get easily readable information and confirmations.
 *
 * ActionAdministrator is Model in MVVM schema.
 * It covers all translations between game logic & game state and
 * player input for both human and AI that needs to use API.
 *
 * This can impose additional restrictions that are not prohibited by raw actions.
 * Most returns from ActionAdministrator are wrapper types.
 *
 * e.g. ActionAdministrator might impose additional restrictions on trading,
 * which normally the action does not cover.
 * This is due to missing validators and instead of having multiple
 * models we are using ActionAdministrator to resolve it.
 */
UCLASS(Blueprintable)
class UNREALDIPLOMACY_API UUDActionAdministrator : public UObject
{
	GENERATED_BODY()
private:
	FUDNationInfo FUDNationInfo_INVALID = FUDNationInfo(-1, -1);
#pragma region Core
public:
	/**
	 * Delegate that is binded by player controller.
	 */
	UserActionRequestedDelegate OnUserActionRequestedDelegate;
	/**
	 * Allows models to subscribe their update function to automatically update.
	 */
	FUDOnDataChanged OnDataChangedEvent;
	/**
	 * Allows models to subscribe their update function to automatically update.
	 */
	FUDOnDataReloaded OnDataReloadedEvent;
	/**
	 * Notifies all viewmodel subscribers about the subtle change.
	 */
	UFUNCTION()
	void OnDataChanged(const FUDActionData& action);
	/**
	 * Notifies all viewmodel subscribers about the reload.
	 */
	UFUNCTION()
	void OnDataReloaded();
	/**
	 * Request action to be send over to server and executed.
	 */
	UFUNCTION()
	void RequestAction(FUDActionData data);
	/**
	 * Defines state that is being monitored and used for determining validity, actions, infos & etc.
	 */
	void SetOverseeingState(TObjectPtr<UUDWorldState> overseeingState);
	/**
	 * Checks if the state functions are useable.
	 */
	bool IsOverseeingStatePresent();
public:
	/**
	 * Returns new action that can be invoked.
	 */
	FUDActionData GetAction(int32 actionId);
	/**
	 * Returns new action that can be invoked.
	 */
	FUDActionData GetAction(int32 actionId, TArray<int32> optionalValues);
	/**
	 * Returns new action that can be invoked.
	 */
	FUDActionData GetAction(int32 actionId, TArray<int32> optionalValues, FString optionalString);
	/**
	 * Returns new action that can be invoked.
	 */
	FUDActionData GetAction(int32 actionId, FString optionalString);
	/**
	 * Returns new action that can be invoked.
	 */
	FUDActionData GetAcceptAction(int32 actionId, FUDActionData sourceAction);
	/**
	 * Returns new action that can be invoked.
	 */
	FUDActionData GetRejectAction(int32 actionId, FUDActionData sourceAction);
protected:
	UPROPERTY()
	TObjectPtr<UUDWorldState> State;
	UPROPERTY()
	TObjectPtr<UUDActionManager> ActionManager;
	/**
	 * Safe retrieve of an action manager.
	 */
	TObjectPtr<UUDActionManager> GetActionManager();
	/**
	 * Initialize new instance for future use with proper action list used in game.
	 */
	void InitializeActionManager();
#pragma endregion
	
#pragma region Lobby & HUD
public:
	/** Provides list of all factions and their names. */
	TArray<FUDFactionMinimalInfo> GetFactionList();
	/** Retrieves local faction name. */
	FString GetLocalFactionName();
	/** Retrieves value from settings. */
	int32 GetSettingsAICount();
	/** Retrieves value from settings. */
	int32 GetSettingsMapSeed();
	/** Retrieves value from settings. */
	int32 GetSettingsMapWidth();
	/** Retrieves value from settings. */
	int32 GetSettingsMapHeight();
	/** Retrieves value from settings. */
	int32 GetSettingsStratagemPoints();
	/** Calculates remaning points left, based on current stratagems. */
	int32 GetLocalStratagemPointsLeft();
	/** Calculates remaning points left, based on current stratagems and returns if it can be bought. */
	bool IsStratagemTakeable(FUDStratagemPickableInfo stratagem);
	/** Provides comprehensive list of all available stratagems, taking them from internal action manager instance. */
	TArray<FUDStratagemPickableInfo> GetStratagemsList();
	/** HUD methord for determining correct UI switching. */
	EUDGameStateInfo GetMatchStateInfo();
private:
	/** Provides comprehensive list of all available stratagems, taking them from internal action manager instance. */
	int32 GetStratagemCostFromTags(TSet<int32> tags);
#pragma endregion

#pragma region GameOver
public:
	/** Checks if game is finished. */
	bool IsGameOver();
	/** Returns full information about the game over state from this perspective. */
	FUDGameOverInfo GetGameOverInfo();
#pragma endregion


#pragma region Faction Interaction
public:	
	/** Provides list of factions and their names if they can be interacted with. */
	TArray<FUDFactionInfo> GetFactionInfoList();
	/** Provides faction name. */
	FUDFactionMinimalInfo GetFactionInfo(int32 factionId);
	/** Provides list of faction interactions. */
	TArray<FUDFactionInteractionInfo> GetFactionInteractionList();
#pragma endregion

#pragma region Resources
public:
	/** Retrieves all resources supported by the game. */
	TArray<FUDResourceInfo> GetResourceList();
	/** Retrieves all resources and their current amount for local player. */
	TArray<FUDResourceInfo> GetLocalFactionResourceList();
	/** Retrieves all resources supported by the game. */
	TArray<FUDResourceMinimalInfo> GetResourceItemList();
#pragma endregion

#pragma region Gameplay
public:
	/** Checks if game is currently played. */
	bool IsGamePlayed();
#pragma endregion

#pragma region Turn
public:
	/** Retrieves all informations related to regent and turn. */
	FUDRegencyTurnInfo GetRegencyTurnInfo();
	/** Return current IsIntermezzo value. Intermezzo describes time, when regency is changed. */
	bool IsIntermezzo();
	/** Return if this faction can finish turn or already finished it. */
	bool CanFinishTurn();
#pragma endregion

#pragma region Diplomacy & Throne
public:
	/** Retrieves all informations related to throne. */
	FUDThroneInfo GetThroneInfo();
	/** Retrieves core informations related to diplomacy. */
	FUDAvailableDiplomacyInfo GetDiplomacyInfo();
	/** Retrieves deal count for unfinished & participating for local player. */
	int32 GetActiveDealParticipationCount();
	/** Retrieves message count for participating. */
	int32 GetUnresolvedMessagesCount();
#pragma endregion

#pragma region Tile & Map Interaction
public:
	/** Retrieves map. */
	TObjectPtr<UUDMapState> GetMapState();
	/** Provides list of tiles and their names if they can be interacted with. */
	//TArray<FUDTileInfo> GetTileInfoList();
	/** Provides tile core informations. */
	FUDTileInfo GetTileInfo(FIntPoint position);
	/** Provides list of tile interactions. */
	TArray<FUDTileInteractionInfo> GetTileInteractionList();
	/** Provides list of all tiles. */
	TArray<FUDTileMinimalInfo> GetTileOptions();
#pragma endregion

#pragma region Parameters
public:
	/**
	 * Takes all parameters from the tags.
	 * Allows to exclude single tag.
	 * Used for removing implicit tags that is not desired to be edited.
	 */
	FUDParameterListInfo GetActionParameters(const TSet<int32>& tags, int32 excludeTag);
	/** Returns list of actions that can be used as parameter. */
	TArray<FUDActionMinimalInfo> GetActionList();
private:
	bool HasDealParameter(const TSet<int32>& tags, int32 excludeTag = UD_INVALID_TAG_ID);
	bool HasFactionParameter(const TSet<int32>& tags, int32 excludeTag = UD_INVALID_TAG_ID);
	bool HasTileParameter(const TSet<int32>& tags, int32 excludeTag = UD_INVALID_TAG_ID);
	bool HasActionParameter(const TSet<int32>& tags, int32 excludeTag = UD_INVALID_TAG_ID);
	bool HasResourceParameter(const TSet<int32>& tags, int32 excludeTag = UD_INVALID_TAG_ID);
	bool HasValueParameter(const TSet<int32>& tags, int32 excludeTag = UD_INVALID_TAG_ID);
	bool HasTextParameter(const TSet<int32>& tags, int32 excludeTag = UD_INVALID_TAG_ID);
	ParameterData GetDealParameter(const TSet<int32>& tags);
	ParameterData GetFactionParameter(const TSet<int32>& tags);
	ParameterData GetTileParameter(const TSet<int32>& tags);
	ParameterData GetActionParameter(const TSet<int32>& tags);
	ParameterData GetResourceParameter(const TSet<int32>& tags);
	ParameterData GetValueParameter(const TSet<int32>& tags);
	ParameterData GetTextParameter(const TSet<int32>& tags);
#pragma endregion

#pragma region Messages & Request Interaction
public:
	/** Retrieves specified request data. */
	FUDActionData GetPendingRequest(int32 pendingRequestId);
	/** Retrieves all request data in presentable format. */
	FUDMessageInteractionInfo GetAllLocalRequests();
private:
	/** Transforms request data into a presentable format. */
	FUDMessageInfo CreateMessageFromRequest(int32 requestId, FUDActionData action);
	/** Merges format and action to result string. */
	FString GetFormattedMessageContent(FString formatString, const TSet<int32>& tags, FUDActionData action);
	/** Retrieves parameters from the tags and associate them with action values. */
	FStringFormatNamedArguments GetMessageContentArguments(const TSet<int32>& tags, FUDActionData action);
	FStringFormatArg GetDealArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetFactionArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetTileArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetActionArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetResourceArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetValueArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetTextArgument(FString data);
#pragma endregion

#pragma region Modifiers
public:
	/** Returns list of modifiers that are part of specified tile. */
	TArray<FUDModifierInfo> GetTileModifierList(FIntPoint tile);
	/** Returns list of modifiers that are part of specified faction. */
	TArray<FUDModifierInfo> GetFactionModifierList(int32 factionId);
private:
	TArray<FUDModifierInfo> GetModifierList(const TArray<FUDModifierData>& modifiers);
	FString GetFormattedModifierContent(FString formatString, const TSet<int32>& tags, FUDModifierData modifier);
	FStringFormatNamedArguments GetModifierContentArguments(const TSet<int32>& tags, FUDModifierData modifier);
#pragma endregion

#pragma region Deals
public:
	/** Returns all active deals separated by active and history for current player. */
	FUDDealInteractionInfo GetAllLocalDeals();
	/** Retrives chat for the local deal. */
	TArray<FUDChatMessageInfo> GetDealChatHistory(int32 dealId);
	/** Returns list of deals that can be used as parameter. */
	TArray<FUDDealMinimalInfo> GetDealList();
	/** Returns extended deal info. */
	FUDDealInfo GetDealInfo(int32 dealId);
	/** Returns list of participants. */
	TArray<FUDDealFactionInfo> GetDealParticipantList(int32 dealId);
	/** Returns list of inviteable. */
	TArray<FUDDealFactionInfo> GetDealInviteList(int32 dealId);
	/** Retrieves all primary points of the specified deal. */
	TArray<FUDDealPointMinimalInfo> GetDealPrimaryPointList(int32 dealId);
private:
	/** Converts state and result to unified name. */
	FText GetStateName(EUDDealSimulationState stae, EUDDealSimulationResult result);
#pragma endregion

public:
	/** Checks if specified faction is owned by local player. */
	bool IsLocalFactionPlayer();
	/** Checks if specified faction is owned & controlled by player or AI. */
	bool IsFactionPlayerControlled(int32 factionId);
	/** 
	 * Checks if specified presentation is stratagem.
	 * If no returns true by default as non-stratagems are always available.
	 * If yes then it checks if it's available to local player and returns based on result. 
	 * TLDR: returns true if player can use specified action.
	 */
	bool IsAvailableStratagem(TSet<int32> tags, int32 actionId);











public:
	/**
	 * Return list of all currently pending requests that require confirmation...
	 */
	UFUNCTION(BlueprintCallable)
		TArray<FUDActionData> GetPendingRequests()
	{
		TArray<FUDActionData> copy;
		for (auto& itm : State->Factions[State->FactionPerspective]->PendingRequests)
		{
			copy.Add(itm.Value);
		}
		return copy;
	}
	/**
	 * Return list of all currently pending requests that require confirmation...
	 */
	UFUNCTION(BlueprintCallable)
		int32 GetPendingRequestsCount()
	{
		return State->Factions[State->FactionPerspective]->PendingRequests.Num();
	}

	//UFUNCTION(BlueprintCallable)
	//	FUDGameStateInfo GetGameStateInfo()
	//{
	//	return FUDGameStateInfo(
	//		State->WorldSimulationState == EUDWorldSimulationState::Finished,
	//		State->ImperialThrone.Ruler
	//	);
	//}

	/**
	 * Alternatively call to IsGameInProgress, that's useable for game that is not yet over.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsGameFinished()
	{
		if (IsOverseeingStatePresent() && 
			State->WorldSimulationState == EUDWorldSimulationState::Finished)
		{
			return true;
		}
		return false;
	}
	/**
	 * Validate if the game is in progress.
	 * Should be called before determining anything else about the world.
	 * Alternatively call IsGameFinished();
	 */
	UFUNCTION(BlueprintCallable)
	bool IsGameInProgress()
	{
		if (IsOverseeingStatePresent() && 
			State->WorldSimulationState == EUDWorldSimulationState::Simulating)
		{
			return true;
		}
		return false;
	}
	/**
	 * Validates if the state has map generated and ready to use.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsMapStateReady()
	{
		return IsValid(State->Map) && State->Map->Tiles.Num() > 0;
	}

	/**
	 * Checks if supplied Id belongs to valid nation.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsValidNation(int32 nationId)
	{
		return State->Factions.Contains(nationId);
	}
	/**
	 * Returns NationInfo.
	 */
	UFUNCTION(BlueprintCallable)
	FUDNationInfo GetCurrentNationState(int32 nationId)
	{
		if (IsValidNation(nationId))
			return FUDNationInfo(nationId, State->Factions[nationId]->Resources[UD_RESOURCE_GOLD_ID]);
		return FUDNationInfo_INVALID;
	}
	/**
	 * Retrieves current gold resource ? TODO this should be converted to generic call for any resource (if we have more then one).
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentResourceGold()
	{
		return State->Factions[State->FactionPerspective]->Resources[UD_RESOURCE_GOLD_ID];
	}
	/**
	 * Returns list of players in PlayerInfo format.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDPlayerInfo> GetPlayerList()
	{
		TArray<FUDPlayerInfo> infos;

		for (auto state : State->Factions)
		{
			FUDPlayerInfo player = FUDPlayerInfo(state.Value->PlayerUniqueId);
			player.Name = FText::Format(LOCTEXT("Player", "Player {0}"), state.Value->PlayerUniqueId);
			infos.Add(player);
		}

		return infos;
	}
	/**
	 * Returns true if it's owned by world.
	 */
	UFUNCTION(BlueprintCallable)
	bool CanTakeTile(FIntPoint position)
	{
		auto tile = State->Map->GetTile(position);
		return tile->OwnerUniqueId == UUDGlobalData::GaiaFactionId;
	}
	/**
	 * Returns true if amount is at least 1 and remains is equal or larger then 0.
	 */
	UFUNCTION(BlueprintCallable)
	bool CanGiveGold(int32 targetId, int32 goldAmount)
	{
		return targetId != State->FactionPerspective 
			&& goldAmount > 0 
			&& State->Factions[State->FactionPerspective]->Resources[UD_RESOURCE_GOLD_ID] - goldAmount >= 0;
	}
	UFUNCTION(BlueprintCallable)
	FIntPoint GetFirstNeutralTile()
	{
		for (auto tile : State->Map->Tiles)
		{
			if (tile->OwnerUniqueId == UUDGlobalData::GaiaFactionId)
			{
				return tile->Position;
			}
		}
		return FIntPoint(-1, -1);
	}

	UFUNCTION(BlueprintCallable)
	bool IsParticipatingInDeal(int32 dealUniqueId)
	{
		if (State->Deals.Num() == 0 || !State->Deals.Contains(dealUniqueId))
		{
			return false;
		}
		TObjectPtr<UUDDealState> lastState = State->Deals[dealUniqueId];
		// verifies if this person is participating in last existing deal.
		return lastState->Participants.Contains(State->FactionPerspective);
	}
	/**
	 * Returns list of active participants, blocked participants, available participants.
	 */
	UFUNCTION(BlueprintCallable)
	FUDDealParticipantsInfo GetDealParticipants(int32 dealUniqueId)
	{
		TArray<FUDPlayerInfo> active;
		TArray<FUDPlayerInfo> blocked;
		TArray<FUDPlayerInfo> available;

		if (State->Deals.Num() == 0 || !State->Deals.Contains(dealUniqueId))
			return FUDDealParticipantsInfo(0, active, blocked, available);

		TObjectPtr<UUDDealState> deal = State->Deals[dealUniqueId];
		TArray<FUDPlayerInfo> players = GetPlayerList();

		for (FUDPlayerInfo info : players)
		{
			if (deal->Participants.Contains(info.Id))
			{
				active.Add(info);
			}
			else if (deal->BlockedParticipants.Contains(info.Id))
			{
				blocked.Add(info);
			}
			else 
			{
				available.Add(info);
			}
		}
		return FUDDealParticipantsInfo(dealUniqueId, active, blocked, available);
	}
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetDealIds()
	{
		TArray<int32> keys;
		keys.Empty(0);

		if (State->Deals.Num() != 0)
		{
			State->Deals.GenerateKeyArray(keys);
			keys.Sort();
		}
		return keys;
	}
	UFUNCTION(BlueprintCallable)
	bool IsModerator(int32 dealUniqueId)
	{
		return State->Deals[dealUniqueId]->OwnerUniqueId == State->FactionPerspective;

	}
	UFUNCTION(BlueprintCallable)
	bool IsParticipantInCurrentDeal(int32 dealUniqueId, int32 playerId)
	{
		if (State->Deals.Num() == 0 || !State->Deals.Contains(dealUniqueId))
			return false;
		if (State->Deals[dealUniqueId]->Participants.Num() == 0)
			return false;
		return State->Deals[dealUniqueId]->Participants.Contains(playerId);
	}

	UFUNCTION(BlueprintCallable)
	bool IsReadyInCurrentDeal(int32 dealUniqueId, int32 playerId)
	{
		if (State->Deals.Num() == 0 || !State->Deals.Contains(dealUniqueId))
			return false;
		if (State->Deals[dealUniqueId]->IsReadyPlayerList.Num() == 0)
			return false;
		return State->Deals[dealUniqueId]->IsReadyPlayerList.Contains(playerId);
	}

	UFUNCTION(BlueprintCallable)
	int32 GetReadyParticipantCount(int32 dealUniqueId)
	{
		if (State->Deals.Num() == 0 || !State->Deals.Contains(dealUniqueId))
			return false;
		if (State->Deals[dealUniqueId]->IsReadyPlayerList.Num() == 0)
			return false;
		return State->Deals[dealUniqueId]->IsReadyPlayerList.Num();
	}

	UFUNCTION(BlueprintCallable)
	bool IsPositiveVotingCurrentDeal(int32 dealUniqueId, int32 playerId)
	{
		if (State->Deals.Num() == 0 || !State->Deals.Contains(dealUniqueId))
			return false;
		if (State->Deals[dealUniqueId]->PositiveVotePlayerList.Num() == 0)
			return false;
		return State->Deals[dealUniqueId]->PositiveVotePlayerList.Contains(playerId);
	}

	UFUNCTION(BlueprintCallable)
	int32 GetPositiveVoteCurrentDealCount(int32 dealUniqueId)
	{
		if (State->Deals.Num() == 0 || !State->Deals.Contains(dealUniqueId))
			return false;
		if (State->Deals[dealUniqueId]->PositiveVotePlayerList.Num() == 0)
			return false;
		return State->Deals[dealUniqueId]->PositiveVotePlayerList.Num();
	}

	UFUNCTION(BlueprintCallable)
	int32 GetParticipantCount(int32 dealUniqueId)
	{
		if (State->Deals.Num() == 0 || !State->Deals.Contains(dealUniqueId))
			return false;
		if (State->Deals[dealUniqueId]->Participants.Num() == 0)
			return false;
		return State->Deals[dealUniqueId]->Participants.Num();
	}

	// structs do not support recursion so we need to make it little less recursive...
	UFUNCTION(BlueprintCallable)
	TArray<FUDDealPointChildInfo> GetDealPointChildTree(int32 dealUniqueId, int32 pointUniqueId)
	{
		if (State->Deals.Num() == 0 ||
			!State->Deals.Contains(dealUniqueId) ||
			State->Deals[dealUniqueId]->Points.Num() == 0 ||
			!State->Deals[dealUniqueId]->Points.Contains(pointUniqueId))
			return TArray<FUDDealPointChildInfo>();

		TArray<FUDDealPointChildInfo> childs;

		for (auto consequencePointId : State->Deals[dealUniqueId]->Points[pointUniqueId]->Consequencies)
		{
			childs.Add(FUDDealPointChildInfo(dealUniqueId, consequencePointId));
		}

		return childs;
	}

	UFUNCTION(BlueprintCallable)
	FUDDealPointTreeInfo GetDealPointsTree(int32 dealUniqueId)
	{
		if (State->Deals.Num() == 0 ||
			!State->Deals.Contains(dealUniqueId))
			return FUDDealPointTreeInfo(0, 0, {});

		TArray<FUDDealPointChildInfo> childs;

		for (auto primaryPointId : State->Deals[dealUniqueId]->PrimaryPoints)
		{
			childs.Add(FUDDealPointChildInfo(dealUniqueId, primaryPointId));
		}
		int32 totalCount = State->Deals[dealUniqueId]->Points.Num();
		return FUDDealPointTreeInfo(dealUniqueId, totalCount, childs);
	}
	UFUNCTION(BlueprintCallable)
	FUDDealPointInfo GetDealPointInfo(int32 dealUniqueId, int32 pointUniqueId)
	{
		if (State->Deals.Num() == 0 ||
			!State->Deals.Contains(dealUniqueId) ||
			!State->Deals[dealUniqueId]->Points.Contains(pointUniqueId))
			return FUDDealPointInfo(0, 0, EUDPointType::Error, 0);
		return FUDDealPointInfo(dealUniqueId, pointUniqueId, 
			State->Deals[dealUniqueId]->Points[pointUniqueId]->Type,
			State->Deals[dealUniqueId]->Points[pointUniqueId]->ActionId,
			State->Deals[dealUniqueId]->Points[pointUniqueId]->ValueParameters,
			State->Deals[dealUniqueId]->Points[pointUniqueId]->TextParameter);
	}

	/**
	 * Returns list of players in PlayerInfo format.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDPlayerInfo> GetInvokerPointPlayerList(int32 dealUniqueId, int32 pointUniqueId)
	{
		TArray<FUDPlayerInfo> infos;

		for (auto playerId : State->Deals[dealUniqueId]->Points[pointUniqueId]->Invokers)
		{
			auto state = State->Factions[playerId];

			FUDPlayerInfo player = FUDPlayerInfo(state->PlayerUniqueId);
			player.Name = FText::Format(LOCTEXT("Player", "Player {0}"), state->PlayerUniqueId);
			infos.Add(player);
		}

		return infos;
	}
	/**
	 * Returns list of players in PlayerInfo format.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDPlayerInfo> GetTargetPointPlayerList(int32 dealUniqueId, int32 pointUniqueId)
	{
		TArray<FUDPlayerInfo> infos;

		for (auto playerId : State->Deals[dealUniqueId]->Points[pointUniqueId]->Targets)
		{
			auto state = State->Factions[playerId];

			FUDPlayerInfo player = FUDPlayerInfo(state->PlayerUniqueId);
			player.Name = FText::Format(LOCTEXT("Player", "Player {0}"), state->PlayerUniqueId);
			infos.Add(player);
		}

		return infos;
	}

	/**
	 * Returns list of actions that will be created from this deal, without any filter.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDActionData> GetDealPointsAsUnfoldedActions(int32 dealUniqueId)
	{
		return UUDDealActionContractCreate::FinalizeActions(State, dealUniqueId);
	}

	UFUNCTION(BlueprintCallable)
	TArray<FUDDealActionInfo> GetAllActionForCurrentPlayer(int32 dealUniqueId)
	{
		TArray<FUDDealActionInfo> actions;
		
		for (size_t index = 0; index < State->Deals[dealUniqueId]->DealActionList.Num(); index++)
		{
			FUDDiscussionAction action = State->Deals[dealUniqueId]->DealActionList[index];
			if (action.Action.InvokerFactionId == State->FactionPerspective) {
				actions.Add(FUDDealActionInfo(dealUniqueId, index, action));
			}
		}

		return actions;
	}

	UFUNCTION(BlueprintCallable)
	TArray<FUDDealActionInfo> GetAllActionForOtherPlayers(int32 dealUniqueId)
	{
		TArray<FUDDealActionInfo> actions;

		for (size_t index = 0; index < State->Deals[dealUniqueId]->DealActionList.Num(); index++)
		{
			FUDDiscussionAction action = State->Deals[dealUniqueId]->DealActionList[index];
			if (action.Action.InvokerFactionId != State->FactionPerspective) {
				actions.Add(FUDDealActionInfo(dealUniqueId, index, action));
			}
		}

		return actions;
	}

	UFUNCTION(BlueprintCallable)
	bool IsDealPointInvoker(int32 dealUniqueId, int32 pointUniqueId, int32 playerId)
	{
		return State->Deals[dealUniqueId]->Points[pointUniqueId]->Invokers.Contains(playerId);
	}
	UFUNCTION(BlueprintCallable)
	bool IsDealPointTarget(int32 dealUniqueId, int32 pointUniqueId, int32 playerId)
	{
		return State->Deals[dealUniqueId]->Points[pointUniqueId]->Targets.Contains(playerId);
	}

	UFUNCTION(BlueprintCallable)
	bool IsLocalPlayerWinner()
	{
		return 
			State->WorldSimulationState == EUDWorldSimulationState::Finished &&
			State->ImperialThrone.Ruler == State->FactionPerspective;
	}

	UFUNCTION(BlueprintCallable)
		bool CanUsurpThrone()
	{
		return State->ImperialThrone.Ruler == UUDGlobalData::GaiaFactionId;
	}

};
#undef LOCTEXT_NAMESPACE