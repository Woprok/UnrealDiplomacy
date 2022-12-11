// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDPlayerController.h"
#include "Core/UDControllerInterface.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include <Skirmish/UDSkirmishGameState.h>
#include "UDSkirmishPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishPlayerController : public AUDPlayerController, public IUDControllerInterface
{
	GENERATED_BODY()
public:
	/**
	 * Default constructor used by engine.
	 */
	//AUDSkirmishPlayerController();
	/**
	 * Allows replication of properties. 
	 * This is used for initial sync as we need to ensure that both
	 * client and server have controller prepared for action.
	 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void SetControllerUniqueId(int32 uniqueControllerId) override;
	virtual int32 GetControllerUniqueId() override;


	/**
	 * Passes received action to world simulation.
	 * Used whenever Multicast is the sender of an action from the server.
	 */
	void ProcessReceivedAction(FUDActionData& actionData);
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
	 * 
	 */
	//void ClientcastInitialSync();
	/**
	 *
	 */
	//void ServercastInitialSync();

private:
	/**
	 * Initializes all fields and prepares all objects for use.
	 */
	void Initialize();
	/**
	 * Simulation that is responsible for maintaining and managing all interactions.
	 */
	UPROPERTY(ReplicatedUsing=OnRep_SetUniqueControllerId)
	int32 UniqueControllerId;
	UFUNCTION()
	void OnRep_SetUniqueControllerId(const int32& oldId);
protected:	
	/**
	 * Lazy access to a WorldSimulation.
	 * Necessary to prevent early call of uninitialized fields.
	 */
	TObjectPtr<AUDWorldSimulation> GetWorldSimulation()
	{
		if (InternalWorldSimulation.IsNull())
		{
			Initialize();
		}
		return InternalWorldSimulation;
	}
	/**
	 * Retrieves current GameState that is associated with the running level.
	 */
	TObjectPtr<AUDSkirmishGameState> GetCastGameState()
	{
		if (InternalCurrentGameState.IsNull())
		{
			InternalCurrentGameState = Cast<AUDSkirmishGameState>(GetWorld()->GetGameState());
		}
		return InternalCurrentGameState;
	}
private:
	/**
	 * Saved pointer for the GameState to reduce amount of access casts.
	 * Access through the GetCastGameState(), this does not have to be initialized.
	 */
	TObjectPtr<AUDSkirmishGameState> InternalCurrentGameState;
	/**
	 * Local simulation that is used by specific client to present current results to user.
	 * This is allowed to only execute actions, that are delivered through the network.
	 * If this would execute local;y created action, it would cause desyncs.
	 *
	 * TODO added handling for premature apply and possible revert on fail to minimize input delay.
	 */
	TObjectPtr<AUDWorldSimulation> InternalWorldSimulation;
};