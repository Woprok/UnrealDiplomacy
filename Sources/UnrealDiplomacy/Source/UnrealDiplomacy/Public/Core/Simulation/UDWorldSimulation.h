// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
#include "UDActionData.h"
#include "UDActionInterface.h"
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
	 * Initializes state that is used only by server and provides source of verification
	 * for consistency of the world.
	 */
	void InitializeGaiaWorldState();
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
	 * Gaia id.
	 */
	const int32 GaiaWorldStateId = 0;
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
private:
	/**
	 * Synchronize new player state to be on par with the old ones.
	 * This will result in modification of all states as new player must be added to them as well.
	 */
	void AUDWorldSimulation::SynchronizeNewPlayerState(TObjectPtr<UUDWorldState> newState);
};
