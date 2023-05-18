// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDGameState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionHandlingInterface.h"
#include "UDSkirmishGameState.generated.h"

// This class is used by both GameMode and PlayerController as middle man.
// Due to that it requires several forward declaration to resolve circular dependencies.
// Forward declaration.
class AUDSkirmishGameMode;

/**
 * Communication middle man between GameMode and PlayerControllers
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishGameState : public AUDGameState
{
	GENERATED_BODY()
public:
	/**
	 * Propagates action to GameMode that will resolve it.
	 * This will cause GameMode to send a response.
	 */
	UFUNCTION()
	void OnServerSendAction(FUDActionData clientData);
	/**
	 * This allows AIController to act by using same channel as remote client with PlayerController.
	 * TODO simplify the register process for controllers.
	 */
	void RegisterActionMaker(TScriptInterface<IUDActionHandlingInterface> newListener);
	/**
	 * Send message to all connected clients.
	 * Server-owned actor with Multicast RPC invoked from the server -> Runs on server and all clients.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSendActionToAllClients(FUDActionData serverData);
	/**
	 * Propagates request for history to GameMode that will resolve it.
	 * Returns wrapped TArray<FUDActionData> of all actions up to the point (optional; 0 = all).
	 * Might be too big ? use with caution... for initial sync only.
	 */
	void ResolveGetSyncData(int32 controllerId, int32 firstKnown);
protected:
	/**
	 * Retrieves current GameMode that is associated with this GameState.
	 */
	TWeakObjectPtr<AUDSkirmishGameMode> GetCastGameMode();
private:
	/**
	 * Saved pointer for the GameMode to reduce amount of access casts.
	 * Access through the GetCastGameMode(), this does not have to be initialized.
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDSkirmishGameMode> InternalCurrentGameMode = nullptr;
};