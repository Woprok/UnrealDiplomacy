// Copyright Miroslav Valach
// TODO Transform ActionAdministrator into multiple models and approriate model manager class.

#pragma once

#include "CoreMinimal.h"
#include "UDActionAdministrator.generated.h"

// Forward Declarations

class IUDActionInterface;
struct FUDActionPresentation;
class UUDWorldState;
class UUDMapState;
class UUDSettingManager;
class UUDActionManager;
class UUDActionDatabase;

enum class EUDDealSimulationState : uint8;
enum class EUDDealSimulationResult : uint8;
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

struct FUDMessageContentInfo;
struct FUDMessageInfo;
struct FUDMessageInteractionInfo;

enum class EUDDecisionType : uint8;
struct FUDDecision;

struct FUDDealParameter;
struct FUDFactionParameter;
struct FUDTileParameter;
struct FUDActionParameter;
struct FUDResourceParameter;
struct FUDValueParameter;
struct FUDTextParameter;
typedef TVariant<FUDDealParameter ,FUDFactionParameter, FUDTileParameter, FUDActionParameter, 
	FUDResourceParameter, FUDValueParameter, FUDTextParameter> ParameterData;

struct FUDDealListInfo;
struct FUDPointInteractionInfo;
struct FUDActionInteractionInfo;
struct FUDChatMessageInfo;
struct FUDDealMinimalInfo;
struct FUDDealInfo;
struct FUDDealFactionInfo;
struct FUDDealPointMinimalInfo;
struct FUDDealActionMinimalInfo;

struct FUDPolicySelectItemInfo;

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
	 * This will exchange Invoker and Target. Thus this expects Target as first value parameter.
	 */
	FUDActionData ReverseActionInvokerAndTarget(FUDActionData actionData);
	/**
	 * Returns new action that can be invoked.
	 */
	FUDActionData GetDecisionAction(int32 targetId, EUDDecisionType type, FUDActionData requestedAction);
	/**
	 * Returns new action that can be invoked.
	 */
	FUDActionData GetConfirmAction(int32 decisionId);
	/**
	 * Returns new action that can be invoked.
	 */
	FUDActionData GetDeclineAction(int32 decisionId);
public:
	/**
	 * Initialize new instance for future use with proper action list used in game.
	 */
	void SetManagers(TWeakObjectPtr<UUDSettingManager> settingManager, TWeakObjectPtr<UUDActionManager> actionManager);
protected:
	UPROPERTY()
	TObjectPtr<UUDWorldState> State;
	UPROPERTY()
	TWeakObjectPtr<UUDSettingManager> SettingManager;
	UPROPERTY()
	TWeakObjectPtr<UUDActionManager> ActionManager;
	/**
	 * Safe retrieve of an action manager.
	 */
	TWeakObjectPtr<UUDActionManager> GetActionManager();
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
	TArray<FUDStratagemPickableInfo> GetPickableStratagemList();
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
	/** Provides list of faction offers. */
	TArray<FUDFactionInteractionInfo> GetFactionOfferList();
	/** Provides list of faction requests. */
	TArray<FUDFactionInteractionInfo> GetFactionRequestList();
	/** Provides list of faction demands. */
	TArray<FUDFactionInteractionInfo> GetFactionDemandList();
	/** Provides list of consequences for faction demands rejection. */
	TArray<FUDFactionInteractionInfo> GetFactionConsequenceList();
	/** Provides list of policies for consequences on demands rejection. */
	TArray<FUDPolicySelectItemInfo> GetConsequencePolicyList();
private:
	/** Provides interactions. */
	TArray<FUDFactionInteractionInfo> CreateFactionInteractionList(TArray<FUDActionPresentation>&& availableActions);
#pragma endregion

#pragma region Resources
public:
	/** Retrieves all resources supported by the game. */
	TArray<FUDResourceInfo> GetResourceList();
	/** Retrieves all resources and their current amount for local player. */
	TArray<FUDResourceInfo> GetLocalFactionResourceList();
	/** Retrieves all resources supported by the game. */
	TArray<FUDResourceMinimalInfo> GetResourceItemList();
	/** Retrieves all resources supported by the game. */
	UMaterialInterface* GetIcon(int32 resourceId);
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
	/** Provides list of all factions that can be on the map. */
	TArray<FUDFactionMinimalInfo> GetMapFactionList();
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
	bool HasDealParameter(const TSet<int32>& tags, int32 excludeTag);
	bool HasFactionParameter(const TSet<int32>& tags, int32 excludeTag);
	bool HasTileParameter(const TSet<int32>& tags, int32 excludeTag);
	bool HasActionParameter(const TSet<int32>& tags, int32 excludeTag);
	bool HasResourceParameter(const TSet<int32>& tags, int32 excludeTag);
	bool HasValueParameter(const TSet<int32>& tags, int32 excludeTag);
	bool HasTextParameter(const TSet<int32>& tags, int32 excludeTag);
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
	FUDActionData GetPendingRequest(int32 pendingDecisionId);
	/** Retrieves all request data in presentable format. */
	FUDMessageInteractionInfo GetAllLocalRequests();
private:
	/** Transforms request data into a presentable format. */
	FUDMessageInfo CreateMessageFromRequest(int32 decisionId, FUDDecision decision);
	/** Merges format and action to result string. */
	FString GetFormattedPresentationString(FString formatString, const TSet<int32>& tags, FUDActionData action);
	/** Retrieves parameters from the tags and associate them with action values. */
	FStringFormatNamedArguments GetPresentationContentArguments(const TSet<int32>& tags, FUDActionData action);
	FStringFormatArg GetDealArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetFactionArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetTileArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetActionArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetResourceArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetValueArgument(const TArray<int32>& data, int32& startIndex);
	FStringFormatArg GetTextArgument(FString data);
	
	FText GetFormattedDecisionType(EUDDecisionType type);
	FUDMessageContentInfo CreateMessageContent(FUDActionData action);
	FUDMessageContentInfo CreateEmptyMessageContent();
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
	FUDDealListInfo GetAllLocalDeals();
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
	TArray<FUDDealActionMinimalInfo> GetDealActionList(int32 dealId);
	/** Retrieves all actions that might be result of this deal. */
	TArray<FUDActionData> GetDealPointsActions(int32 dealId);
	/** Retrieves single point that can be used as editable content. */
	FUDPointInteractionInfo GetPointInteraction(int32 dealId, int32 pointId);
	/** Retrieves single point that can be used as executable content. */
	FUDActionInteractionInfo GetActionInteraction(int32 dealId, int32 indexId);
	/** Same as action parameters, but always includes special editable values for deals. */
	FUDParameterListInfo GetDealActionParameters(const TSet<int32>& tags);
private:
	/** Retrieves action parameters for editing with their value. */
	FUDParameterListInfo GeActionParametersWithValues(const TSet<int32>& tags, FUDActionData data);
	/** Retrieves deal action parameters for editing with their value. */
	FUDParameterListInfo GetDealActionParametersWithValues(const TSet<int32>& tags, int32 dealAction, FUDActionData data);
	/** Converts state and result to unified name. */
	FText GetStateName(EUDDealSimulationState stae, EUDDealSimulationResult result);
#pragma endregion

public:
	/** Checks if this state is owned by local player (not an observer or server). */
	bool IsLocalFactionPlayer();
	/** Checks if specified faction is owned & controlled by player or AI. */
	bool IsFactionPlayerControlled(int32 factionId);
	/** Checks if local player has access to specified action from any source, including not being stratagem. */
	bool HasActionOrStratagem(int32 actionId);
	/** 
	 * Checks if specified presentation is stratagem.
	 * If no returns true by default as non-stratagems are always available.
	 * If yes then it checks if it's available to local player and returns based on result. 
	 * TLDR: returns true if player can use specified action.
	 */
	bool IsAvailableStratagem(const TSet<int32>& tags, int32 actionId);
	/** Checks if specified faction is same as neutral. */
	bool IsNeutral(int32 factionId);
#pragma region AI Checks
	/** Returns all neutral tiles. */
	TArray<FUDTileMinimalInfo> GetNeutralTiles();
#pragma endregion
};
#undef LOCTEXT_NAMESPACE