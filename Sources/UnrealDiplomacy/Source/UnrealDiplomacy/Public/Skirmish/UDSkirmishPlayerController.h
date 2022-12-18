// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDPlayerController.h"
#include "Core/UDControllerInterface.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "UDSkirmishGameState.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include "UDSkirmishPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishPlayerController : public AUDPlayerController, public IUDControllerInterface
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void OnRep_SetUniqueControllerId(const int32& oldId);
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
	UFUNCTION(Server, Reliable)
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
public:
	/**
	 * Allows replication of properties.
	 * This is used for initial sync as we need to ensure that both
	 * client and server have controller prepared for action.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	virtual void SetControllerUniqueId(int32 uniqueControllerId) override
	{
		UniqueControllerId = uniqueControllerId;
	}
	UFUNCTION()
	virtual int32 GetControllerUniqueId() override
	{
		return UniqueControllerId;
	}
public:
	/**
	 * Executed once a client has finished synchronization.
	 * Allows UI elements to read state and enabled generation of game world from input.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSynchronizationFinished();
public:
	/**
	 * Passes received action to world simulation.
	 * Used whenever Multicast is the sender of an action from the server.
	 */
	void MulticastReceiveActionFromServer_Local(FUDActionData& actionData);
protected:
	/**
	 * Initializes Overseer for handling game rules over this player.
	 */
	void InitializeAdministrator();
	/**
	 * Accessor for blueprints and others to PersonalAdministrator of this controller.
	 */
	UFUNCTION(BlueprintCallable)
	UUDActionAdministrator* GetPersonalAdministrator()
	{
		return InternalPersonalAdministrator;
	}
		/**
	 * Initializes all fields and prepares all objects for use.
	 */
	void InitializeSimulation();
	/**
	 * Lazy access to a WorldSimulation.
	 * Necessary to prevent early call of uninitialized fields.
	 */
	TWeakObjectPtr<AUDWorldSimulation> GetWorldSimulation()
	{
		if (!InternalWorldSimulation.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController: New simulation required."));
			InitializeSimulation();
		}
		return InternalWorldSimulation;
	}
	/**
	 * Retrieves current GameState that is associated with the running level.
	 */
	TWeakObjectPtr<AUDSkirmishGameState> GetCastGameState()
	{
		if (!InternalCurrentGameState.IsValid())
		{
			InternalCurrentGameState = Cast<AUDSkirmishGameState>(GetWorld()->GetGameState());
		}
		return InternalCurrentGameState;
	}
	/**
	 * Determines how to deal with the action and executes it if possible.
	 * Requires client to be synchronized.
	 */
	void ProcessAction(FUDActionData& actionData);
	/**
	 * Saves unresolveable actions for later.
	 */
	void SaveActionUntilSynchronization(FUDActionData& actionData);
	/**
	 * Verifies that sync is requested and if it is not then start it.
	 * Invoked on receiving new action from standard channel.
	 */
	void VerifySyncInProgress();
	/**
	 * Starts initial sync by sending new request to server.
	 */
	void StartSynchronization();
	/**
	 * Process all data received...
	 */
	void FinishSynchronization(FUDActionArray& actionArray);
private:
	UPROPERTY(ReplicatedUsing = OnRep_SetUniqueControllerId)
	int32 UniqueControllerId;
	/**
	 * Saved pointer for the GameState to reduce amount of access casts.
	 * Access through the GetCastGameState(), this does not have to be initialized.
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDSkirmishGameState> InternalCurrentGameState = nullptr;
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
	 * Determines if this client asked & finished for initial history catch-up.
	 */
	UPROPERTY()
	bool IsSynchronized = false;
	/**
	 * Determines if this client asked for initial history catch-up.
	 */
	UPROPERTY()
	bool IsSyncInProgress = false;
	/**
	 * Holds history and all actions received, until this client was fully synchronized.
	 */
	UPROPERTY()
	TArray<FUDActionData> TemporaryActionHolder;
	/**
	 * Handles logic verfication and game rules access over the state owned by this controller.
	 */
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> InternalPersonalAdministrator = nullptr;
};