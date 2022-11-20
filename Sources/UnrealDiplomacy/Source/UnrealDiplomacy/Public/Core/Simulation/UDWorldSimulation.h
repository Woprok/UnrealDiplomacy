// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
#include "UDActionData.h"
#include "UDActionInterface.h"
#include "UDActionHandlingInterface.h"
#include "UDWorldState.h"
#include "UDWorldSimulation.generated.h"

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
	 * Allows controllers to register for incoming messages from the simulation about the state change.
	 * Invoked everytime an action was executed by the simulation.
	 */
	DECLARE_MULTICAST_DELEGATE_OneParam(BroadcastActionExecutedDelegate, FUDActionData&);
	/**
	 * Delegate for broadcast of an action being executed, thus resulting in state changing.
	 * Action that was done is passed ?
	 * TODO reevaluate restraints.
	 */
	BroadcastActionExecutedDelegate OnBroadcastActionExecutedDelegate;
	/**
	 * This allows controller to respond for each action that it received if deemed necessary.
	 * TODO simplify the register process for controllers.
	 */
	void RegisterActionMaker(TObjectPtr<IUDActionHandlingInterface> newListener);

	/**
	 * Initializes state that is used only by server and provides source of verification
	 * for consistency of the world.
	 */
	void InitializeGaiaWorldState(int32 gaiaId);
	/**
	 * Creates new state that will be simultenuosly simulated along with all others.
	 * Note: Player and Ai are both considered player. Only world/gaia is considered non-player.
	 */
	void CreateState(int32 playerId, bool isPlayerOrAi);
	/**
	 * Register new action that can be executed over this simulation.
	 */
	void RegisterAction(TObjectPtr<IUDActionInterface> newAction);
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
protected:
	/**
 	 * Each Player/Ai and Server have their own instance/state of the world.
	 */
	TMap<int32, TObjectPtr<UUDWorldState>> States;
	/**
	 * Gaia state shortcut. Exists only on server.
	 */
	TObjectPtr<UUDWorldState> GaiaState;
	/**
	 * List of all actions registered in this simulation.
	 */
	TMap<int32, TObjectPtr<IUDActionInterface>> Actions;
	/**
	 * List of all actions in chronological order of execution over the GaiaState.
	 * Due to that it's necessary tool for synchronizing new players as well as old, if
	 * desync would ever happen.
	 */
	TArray<FUDActionData> ExecutionHistory;
	/**
	 * List of all actions that were reverted.
	 * TODO improve this, if there will be good reason to make it a full feature.
	 */
	TArray<FUDActionData> UndoHistory;
private:
	/**
	 * Synchronize new player state to be on par with the old ones.
	 * This will result in modification of all states as new player must be added to them as well.
	 */
	void SynchronizeNewPlayerState(TObjectPtr<UUDWorldState> newState);
};
