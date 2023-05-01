// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
#include "UDActionData.h"
#include "UDWorldArbiter.h"
#include "UDWorldGenerator.h"
#include "UDModifierManager.h"
#include "UDActionInterface.h"
#include "UDActionHandlingInterface.h"
#include "UDWorldState.h"
#include "UDWorldSimulation.generated.h"

class UUDActionManager;

/**
 * Allows controllers to register for incoming messages from the simulation about the state change.
 * Invoked everytime an action was executed by the simulation.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(BroadcastActionExecutedDelegate, FUDActionData&);

/**
 * Handles all operations over the UDWorldState and 
 * holds all Player/Ai and server states.
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
	/**
	 * Delegate for broadcast of an action being executed, thus resulting in state changing.
	 * Action that was done is passed ?
	 * TODO reevaluate restraints.
	 */
	BroadcastActionExecutedDelegate OnBroadcastVerifiedActionExecutedDelegate;
	/**
	 * Simplified version of OnBroadcastVerifiedActionExecutedDelegate
	 * Invoked by the other execution function, that does no verification NaiveExecuteAction.
	 */
	BroadcastActionExecutedDelegate OnBroadcastActionAppliedDelegate;
	/**
	 * Creates new state that will be simultenuosly simulated along with all others.
	 * Note: Player and Ai are both considered player. Only world/gaia is considered non-player.
	 * Automatically synchronizes data with this state.
	 */
	void CreateStateAndSynchronize(int32 playerId, bool isPlayerOrAi);
	/**
	 * Creates new state that will be simultenuosly simulated along with all others.
	 * Note: Player and Ai are both considered player. Only world/gaia is considered non-player.
	 * Creates only empty state.
	 */
	void CreateState(int32 playerId, bool isPlayerOrAi);
	/**
	 * Verifies viability of the action and executes it.
	 * TODO FAIL TYPE
	 */
	void ExecuteAction(FUDActionData& newAction);
	/**
	 * Reverse execution of single action.
	 * Can be called repeateadly to revert all actions.
	 * Can and will break execution of used too much.
	 * TODO improve this, if there will be good reason to make it a full feature.
	 */
	void RevertAction();
	/**
	 * Returns state for read only purposes. Exposed for controllers.
	 */
	TObjectPtr<UUDWorldState> GetSpecificState(int32 stateOwnerId)
	{
		if (States.Contains(stateOwnerId))
		{
			return States[stateOwnerId];
		}
		return nullptr;
	};
	/**
	 * Creates copy of history until the specified point.
	 * 0 = all
	 */
	FUDActionArray GetHistoryUntil(int32 historyPoint);
	/**
	 * Executes action without any verification.
	 * Useful for actions that are believed to be absolutely correct. e.g. previously executed.
	 */
	void NaiveExecuteAction(FUDActionData& trustworthyAction);
private:
	/**
	 * Synchronize new player state to be on par with the old ones.
	 * This will result in modification of all states as new player must be added to them as well.
	 */
	void SynchronizeNewPlayerState(TObjectPtr<UUDWorldState> newState);
protected:
	/**
	 * Returns true if the current id is proper, otherwise false.
	 * Id with value of 0 is not valid.
	 */
	bool IsValidAssignableActionId(int32 currentId)
	{
		return currentId != 0;
	}
	/**
	 * Id assigned to actions, that were passed for execution. If action already has an id,
	 * it will not get new id.
	 * Id with value 0 is considered invalid.
	 * TODO maybe we should not really on 0 as invalid value :)
	 */
	int32 GetAssignableActionId()
	{
		++AssignableActionId;
		while (AssignableActionId == 0)
		{
			// returns can be only non 0 value.
			++AssignableActionId;
		}
		// finally returning non 0 value.
		return AssignableActionId;
	}
private:
	/**
	 * Each Player/Ai and Server have their own instance/state of the world.
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UUDWorldState>> States;
	/**
	 * List of all actions in chronological order of execution over the GaiaState.
	 * Due to that it's necessary tool for synchronizing new players as well as old, if
	 * desync would ever happen.
	 */
	UPROPERTY()
	TArray<FUDActionData> ExecutionHistory;
	/**
	 * List of all actions that were reverted.
	 * TODO improve this, if there will be good reason to make it a full feature.
	 */
	UPROPERTY()
	TArray<FUDActionData> UndoHistory;
	/**
	 * Current modifier manager used by this simulation.
	 */
	UPROPERTY()
	TObjectPtr<UUDWorldArbiter> Arbiter = nullptr;
	UPROPERTY()
	TObjectPtr<UUDActionManager> ActionManager = nullptr;
	/**
	 * Id assigned to actions, that were passed for execution. If action already has an id,
	 * it will not get new id.
	 * Id with value 0 is considered invalid.
	 */
	UPROPERTY()
	int32 AssignableActionId = 0;
};