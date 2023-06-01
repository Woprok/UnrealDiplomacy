// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDPlayerController.h"
#include "UDSkirmishPlayerController.generated.h"

// Forward Declarations

struct FUDActionData;
class AUDSkirmishGameState;
class UUDActionAdministrator;
class AUDWorldSimulation;
class AUDSquareGrid;

/**
 * Defines current state of synchronization.
 */
UENUM(BlueprintType)
enum class EUDSynchronizationState : uint8
{
	Undefined,
	SynchronizingData,
	Synchronized
};

/**
 * Defines client side part of communication protocol between client and server.
 * Exposes data to UI and World. Handles World Input and executes requests from UI Input.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishPlayerController : public AUDPlayerController
{
	GENERATED_BODY()
#pragma region Client-Server RPCs
public:
	/**
	 * Passes received action to world simulation.
	 * Used whenever Multicast is the sender of an action from the server.
	 * Invoked by GameState during the Multicast broadcast.
	 */
	void MulticastReceiveActionFromServer_Local(FUDActionData& action);
	/**
	 * Receives action from server to this client.
	 * Client-owned actor with Server RPC invoked from the server -> Runs on actor's owning client.
	 */
	UFUNCTION(Client, Reliable)
	void ClientcastReceiveActionFromServer(FUDActionData serverData);
	/**
	 * Sends action to server from this client.
	 * Owned by invoking client with Client RPC invoked from a client -> Runs on server.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServercastSendActionToServer(FUDActionData clientData);
	/**
	 * Receives sync data from server.
	 * Client-owned actor with Server RPC invoked from the server -> Runs on actor's owning client.
	 */
	UFUNCTION(Client, Reliable)
	void ClientcastInitialSyncReceiveFromServer(FUDActionArray actions);
	/**
	 * Requests sync data from sever.
	 * Owned by invoking client with Client RPC invoked from a client -> Runs on server.
	 * Parameters:
	 * controllerId
	 * firstKnwon - optional (0 = all) id of first already owned action.
	 */
	UFUNCTION(Server, Reliable)
	void ServercastInitialSyncRequestToServer(int32 controllerId, int32 firstKnown);
protected:
	/**
	 * Retrieves current GameState that is associated with the running level and handles RPCs.
	 */
	TWeakObjectPtr<AUDSkirmishGameState> GetCastGameState();
private:
	/**
	 * Saved pointer for the GameState to reduce amount of access casts.
	 * Access through the GetCastGameState(), this does not have to be initialized or valid.
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDSkirmishGameState> InternalCurrentGameState = nullptr;
#pragma endregion

#pragma region Synchronization
protected:
	virtual void StartClientSynchronization() override;
	virtual void StartFactionChange() override;
	/**
	 * Starts initial sync by sending new request to server.
	 */
	void StartJoinDataSynchronization();
	/**
	 * Process all data received from server.
	 * Invoked on receiving new action from synchronization channel.
	 */
	void FinishDataSynchronization(FUDActionArray& actionArray);
	/**
	 * Determines how to deal with the action and executes it if possible.
	 * Requires client to be synchronized, otherwise it will store action and attempt to synchronize.
	 */
	void ReceiveAction(FUDActionData& action);
private:
	/**
	 * Saves action to be executed later.
	 */
	void StoreAction(FUDActionData& action);
	/**
	 * Verifies that sync is requested and if it is not then start it.
	 * Invoked on receiving new action from standard channel.
	 */
	void AttemptSynchronization();
	/**
	 * Returns invalid or existing action id recevied from server.
	 */
	int32 GetFirstKnownActionId();
	/**
	 * Determines state of synchronization.
	 */
	UPROPERTY()
	EUDSynchronizationState SynchronizationState = EUDSynchronizationState::Undefined;
	/**
	 * Holds history and all actions received, until this client was fully synchronized.
	 */
	UPROPERTY()
	TArray<FUDActionData> StoredActions;
#pragma endregion

#pragma region Action Execution
	void ChangeFaction();
	void RunAction(FUDActionData& actionData);
	void RunAllActions(TArray<FUDActionData>& actionArray);
#pragma endregion

#pragma region Player Input & UI

#pragma endregion



public:
	/**
	 * Called by whoever is responsible for propagating UI requests.
	 */
	void OnUserActionRequested(FUDActionData requestedAction);
	/**
	 * Executed once a client has finished synchronization.
	 * Allows UI elements to read state and enabled generation of game world from input.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnSynchronizationFinished();
	/**
	 * Invoked by simulation finishing processing incoming change.
	 * All Widgets and World needs to check if they can update themselves from the state change.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnWorldStateUpdated();
protected:
	/**
	 * Binding to delegate of WorldSimulation.
	 */
	void OnWorldSimulationUpdated(FUDActionData& action);
	/**
	 * Initializes Overseer for handling game rules over this player.
	 */
	void InitializeAdministrator();
	/**
	 * Accessor for blueprints and others to PersonalAdministrator of this controller.
	 */
	UFUNCTION(BlueprintCallable)
	UUDActionAdministrator* GetAdministrator();
	/**
	 * Initializes all fields and prepares all objects for use.
	 */
	void InitializeSimulation();
	/**
	 * Lazy access to a WorldSimulation.
	 * Necessary to prevent early call of uninitialized fields.
	 */
	TWeakObjectPtr<AUDWorldSimulation> GetWorldSimulation();

	/**
	 * Creates instance and setups all relevant links to players controller.
	 */
	void InitializeGrid();
private:
	/**
	 * Simulation that is responsible for maintaining and managing all interactions.
	 * Local simulation that is used by specific client to present current results to user.
	 * This is allowed to only execute actions, that are delivered through the network.
	 * If this would execute localy created action, it would cause desyncs.
	 *
	 * TODO added handling for premature apply and possible revert on fail to minimize input delay.
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDWorldSimulation> InternalWorldSimulation = nullptr;

	/**
	 * Handles logic verfication and game rules access over the state owned by this controller.
	 */
	UPROPERTY()
		TObjectPtr<UUDActionAdministrator> InternalPersonalAdministrator = nullptr;
protected:
	/**
	 * World representation.
	 */
	UPROPERTY(BlueprintReadOnly)
		TWeakObjectPtr<AUDSquareGrid> SquareGrid = nullptr;
	/**
	 * Grid blueprint used by this controller.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkirmishController|Config")
		TSubclassOf<AUDSquareGrid> GridBlueprintClass;
};