// Copyright Miroslav Valach
// TODO change ActionId to long, so there is even smaller chance of someone using all action Ids.
// TODO create client version of this class.

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
#include "UDWorldSimulation.generated.h"

// Forward Declarations

enum class EUDWorldPerspective : uint8;
struct FUDActionData;
struct FUDActionArray;
class IUDControllerInterface;
class IUDActionInterface;
class UUDActionManager;
class UUDWorldArbiter;
class UUDWorldState;

/**
 * Allows controllers to register for incoming messages from the simulation about the state change.
 * Invoked everytime an action was executed by the simulation.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(BroadcastActionExecutedDelegate, FUDActionData&);

/**
 * Simulation that is responsible for maintaining and managing all interactions.
 * Basically all actions go through this before they are processed and send to clients.
 * Clients utilize specific function to execute naively without checks everything received from server.
 * Server on other hand has function that always checks validity of the action on Gaia Faction State.
 * Due to this client should never execute actions that were not received from server as it would cause desyncs.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDWorldSimulation : public AUDActor
{
	GENERATED_BODY()
public:
	/**
	 * Initialize all necessary fields for actions and simulation.
	 */
	void Initialize();
#pragma region Assigns of controllers to specific state.
public:
	/**
	 * Checks if the specified faction is associated with player or is non-playable faction.
	 */
	bool IsPlayerFaction(int32 factionId);
	/**
	 * Faction for Gaia / Server.
	 * This will never create more then one instance.
	 */
	int32 CreateGaiaFaction();
	/**
	 * Faction for Observer.
	 * This will never create more then one instance.
	 */
	int32 CreateObserverFaction();
	/**
	 * Player faction is unique for each player.
	 */
	int32 CreatePlayerFaction();
	/**
	 * Creates faction state for local purposes.
	 * This is used only on client.
	 */
	void CreatePrivatePlayerFaction(int32 factionId);
	/**
	 * Promote faction to all connected clients after creating it.
	 * Use on server only.
	 */
	void AnnounceFaction(int32 factionId);
protected:
	/**
	 * Synchronizes specified state by
	 * -> updating state from history.
	 * -> executing relevant actions such as join.
	 * Note: Player and Ai are both considered player. Only world/gaia is considered non-player.
	 * Automatically synchronizes data with this state.
	 */
	void CreateSynchronizedState(int32 factionId, EUDWorldPerspective perspective);
	/**
 	 * Provides Id to agents, that we have full control over.
	 */
	int32 GetNewFactionId();
private:
	/**
	 * Fully initializes new faction state as part of simulation.
	 * All factions states are by default empty.
	 */
	void CreateNewState(int32 factionId, EUDWorldPerspective perspective);
	/**
	 * Synchronizes specified state by applying all actions in ExecutionHistory.
	 */
	void SynchronizeNewFactionState(TObjectPtr<UUDWorldState> newFactionState);
	/**
	 * Announces to everyone that new faction (player) was added to the game world.
	 */
	void CreateFactionCreationEvent(int32 factionId);
	UPROPERTY()
	int32 NextUniqueFactionId;
#pragma endregion

#pragma region Data
public:
	/**
	 * Returns state for read only purposes. Exposed for controllers.
	 */
	TObjectPtr<UUDWorldState> GetFactionState(int32 factionId);
private:
	/**
	 * All factions have their own state, that contains everything from their point of view.
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UUDWorldState>> States;
	/**
	 * Chronological order of all actions that were correct and completely executed.
	 * If action is reverted then it needs to be removed from this list.
	 * Reverting is only allowed for last available action in this list.
	 * Beaware, if this rules are not followed new factions will start pernamently desynced.
	 */
	UPROPERTY()
	TArray<FUDActionData> ExecutionHistory;
	/**
	 * Arbiter used to check on simulation execution state.
	 */
	UPROPERTY()
	TObjectPtr<UUDWorldArbiter> Arbiter = nullptr;
	/**
	 * Manager that is responsible for providing actions for execution.
	 */
	UPROPERTY()
	TObjectPtr<UUDActionManager> ActionManager = nullptr;
#pragma endregion

#pragma region Actions
public:
	/**
	 * Executes action without any verification.
	 * Useful for actions that were already executed.
	 * e.g client executing action from server...
	 * This is sub-call of the CheckAndExecuteAction.
	 */
	void OnlyExecuteAction(FUDActionData& trustworthyAction);
	/**
	 * Properly checks and only then executes action.
	 * Actions that fail the check are discarded.
	 */
	void CheckAndExecuteAction(FUDActionData& newAction);
	/**
	 * Delegate for broadcast of an action being executed, thus resulting in state changing.
	 * This is executed after the other broadcast. 
	 * Always indicates that action was also verified before execution.
	 */
	BroadcastActionExecutedDelegate OnBroadcastVerifiedActionExecutedDelegate;
	/**
	 * Simplified version of OnBroadcastVerifiedActionExecutedDelegate
	 * Invoked by OnlyExecuteAction after it finishes.
	 */
	BroadcastActionExecutedDelegate OnBroadcastActionAppliedDelegate;
protected:
	/**
	 * Checks if this id is considerd invalid as defined by UUDGlobalData
	 */
	bool HasValidActionId(int32 currentId);
	/**
	 * Each action that is being processed will receive new unique id.
	 * Actions have invalid id defined in UUDGlobalData.
	 */
	int32 GetNewActionId();
private:
	/**
	 * Id assigned to actions, that were passed for execution. If action already has an id,
	 * it will not get new id.
	 * Id with value 0 is considered valid.
	 */
	UPROPERTY()
	int32 NextUniqueActionId;
#pragma endregion

#pragma region CheckAndExecute Helpers
	void AssignActionIds(FUDActionData& newAction);

	void CreateActionBackup(FUDActionData& newAction,
		TScriptInterface<IUDActionInterface>& actionExecutor,
		TObjectPtr<UUDWorldState>& gaiaFactionState);

	void RunActionContinuations(FUDActionData& newAction,
		TScriptInterface<IUDActionInterface>& actionExecutor,
		TObjectPtr<UUDWorldState>& gaiaFactionState);

	void PostActionStateCheck(int32 actionExecutorId,
		TObjectPtr<UUDWorldState>& gaiaFactionState);
#pragma endregion

#pragma region Server Sync
public:
	/**
	 * Creates copy of history until the specified point.
	 * If UUDGlobalData InvalidActionId is provied this will return all actions.
	 */
	FUDActionArray GetHistoryUntil(int32 historyPoint);
#pragma endregion

#pragma region Revert
	// TODO Revert functionality and testing.
	// - There is issue with how is this supposed to work in multiplayer environment.
	// - AI is not aware of this system.
	// - AI needs to be able to store data and backtrack if this is used.
	// - This would require either strict client side simulation.
	// - Or turns would have to prevent all others players from acting.
	// - Current turn implementation is simultaneus and thus not suitable for this.
	// - Current implementation allows only last played action to be removed.
public:
	/**
	 * Reverse execution of single action.
	 * Can be called repeateadly to revert all actions.
	 * This is experimental feature and it's not stable.
	 * Currently AI will not work properly with it.
	 */
	void RevertAction();
private:
	/**
	 * List of all actions that were reverted.
	 */
	UPROPERTY()
	TArray<FUDActionData> UndoHistory;
#pragma endregion
};