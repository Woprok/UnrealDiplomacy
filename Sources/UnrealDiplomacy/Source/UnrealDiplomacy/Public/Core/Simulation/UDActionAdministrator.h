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
struct FUDDealActionInfo;

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

// TODO rework this
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
	int32 DealId;
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
	/** Checks if map state is present and can be used. */
	bool IsMapStatePresent();
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
	/** Retrieves all secondary points of the specified deal. */
	TArray<FUDDealPointMinimalInfo> GetDealSecondaryPointList(int32 dealId, int32 pointId);
	/** Retrieves all tertiary points of the specified deal. */
	TArray<FUDDealPointMinimalInfo> GetDealTertiaryPointList(int32 dealId, int32 pointId);
	/** Retrieves all actions associated with the specified deal for execution. */
	TArray<FUDDealActionInfo> GetDealActionList(int32 dealId);
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
	/** Checks if specified faction is same as neutral. */
	bool IsNeutral(int32 factionId);
#pragma region AI Checks
	/** Returns all neutral tiles. */
	TArray<FUDTileMinimalInfo> GetNeutralTiles();
#pragma endregion


public:
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
};
#undef LOCTEXT_NAMESPACE