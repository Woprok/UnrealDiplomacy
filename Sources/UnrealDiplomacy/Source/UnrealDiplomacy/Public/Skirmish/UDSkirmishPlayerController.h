// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDPlayerController.h"
#include "UDSkirmishPlayerController.generated.h"

// Forward Declarations

struct FUDActionData;
struct FUDCommandData;
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
public:
	AUDSkirmishPlayerController();
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
	/**
	 * Sends command to server from this client.
	 * Owned by invoking client with Client RPC invoked from a client -> Runs on server.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServercastSendCommandToServer(FUDCommandData clientData);
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
private:
	void ChangeFaction();
	void RunAction(FUDActionData& actionData);
	void RunAllActions(TArray<FUDActionData>& actionArray);
private:
	/**
	 * Initializes all fields and prepares all objects for use.
	 */
	void InitializeSimulation();
	/**
	 * Initializes Overseer for handling game rules over this player.
	 */
	void InitializeAdministrator();
	/**
	 * Lazy access to a WorldSimulation.
	 * Necessary to prevent early call of uninitialized fields.
	 */
	TWeakObjectPtr<AUDWorldSimulation> GetWorldSimulation();
	/**
	 * Accessor for blueprints and others to Administrator of this controller.
	 */
	UFUNCTION(BlueprintCallable)
	UUDActionAdministrator* GetAdministrator();
	/**
	 * Local version of the simulation. 
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDWorldSimulation> InternalWorldSimulation = nullptr;
	/**
	 * Handles logic verfication and game rules access over the state owned by this controller.
	 */
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> InternalPersonalAdministrator = nullptr;
#pragma endregion

#pragma region Player Input & UI & Updates
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
	 * Binding to delegate of WorldSimulation.
	 */
	void OnSynchronizationCompleted();
#pragma endregion

#pragma region World
protected:
	/**
	 * Creates instance and setups all relevant links to players controller.
	 */
	void InitializeGrid();
protected:
	/**
	 * World representation.
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AUDSquareGrid> SquareGrid = nullptr;
	/**
	 * Grid blueprint used by this controller and thus determines how will world look and operate on data.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkirmishController|Config")
	TSubclassOf<AUDSquareGrid> GridBlueprintClass;
#pragma endregion
};