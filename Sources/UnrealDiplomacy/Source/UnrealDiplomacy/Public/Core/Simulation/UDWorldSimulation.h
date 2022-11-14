// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
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
protected:
	/**
 	 * Each Player/Ai and Server have their own instance/state of the world.
	 */
	TMap<int32, TObjectPtr<UUDWorldState>> States;
	/**
	 * Gaia state shortcut. Exists only on server.
	 */
	TObjectPtr<UUDWorldState>& GaiaState;
	/**
	 * Gaia id.
	 */
	const int32 GaiaWorldStateId = 0;
	/**
	 * Gaia state shortcut. Exists only on server.
	 */
	TMap<int32, TObjectPtr<IUDActionInterface>> Actions;
};
