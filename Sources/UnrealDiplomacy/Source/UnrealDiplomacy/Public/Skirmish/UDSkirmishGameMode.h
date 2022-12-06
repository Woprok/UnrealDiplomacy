// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDGameMode.h"
#include "Core/UDControllerInterface.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include "UDSkirmishGameState.h"
#include "UDSkirmishHUD.h"
#include "UDSkirmishAIController.h"
#include "UDSkirmishGaiaAIController.h"
#include "UDSkirmishPlayerController.h"
#include "UDSkirmishPlayerState.h"
#include "UDSkirmishGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishGameMode : public AUDGameMode
{
	GENERATED_BODY()
public:
	AUDSkirmishGameMode();
	/**
	 * Process action that was invoked by Client. TODO as well for AI ?
	 */
	virtual void ProcessAction(FUDActionData& actionData);
protected:
	/**
	 * Handles decision how to propagate action that was successfully executed.
	 * GameMode handles passing notification and action to all Clients and AIs.
	 */
	virtual void ActionExecutionFinished(FUDActionData& actionData);
	/**
	 * Required to be called as last thing during game setup.
	 * This will invoke UUDStartGameAction that handles pre-first turn play.
	 */
	virtual void StartGame();
	/**
	 * Creates sufficient amount of Ai controllers.
	 */
	virtual void CreateAiPlayers(int32 count);
	/**
	 * Invokes call to register newly joined PlayerController for a game.
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/**
	 * Provides Id to agents, that we have full control over.
	 */
	int32 GetNextUniqueControllerId();
	/** 
	 * Instance of Gaia. 
	 */
	TObjectPtr<AUDSkirmishGaiaAIController> GaiaController;
	/**
	 * List of recognized bots.
	 */
	TArray<TObjectPtr<AUDSkirmishAIController>> AiControllers;
	/**
	 * List of recognized players.
	 */ 
	TArray<TObjectPtr<AUDSkirmishPlayerController>> PlayerControllers;
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
private:
	/**
	 * Initializes all fields and prepares all objects for use.
	 */
	void Initialize();
	/**
	 * Simulation that is responsible for maintaining and managing all interactions.
	 */
	TObjectPtr<AUDWorldSimulation> InternalWorldSimulation;
	/**
	 * Registers listen to world simulation owned by this GameMode.
	 */
	virtual void RegisterAsListenerToWorldSimulation();
	/**
	 * Assigns state to Gaia.
	 */
	void RegisterGaiaAi();
	/**
	 * Assigns state to Ai and saves Ai in AiControllers.
	 */
	void RegisterAi();
	/**
	 * Create Ai by initializing new AiController.
	 */
	virtual TObjectPtr<AUDSkirmishAIController> CreateAi();
	/**
	 * Assigns state to Player and saves Player in PlayerControllers.
	 */
	void RegisterPlayer(APlayerController* NewPlayer);
	/**
	 * Each controller posses unique Id, that is assigned to it via interface.
	 * This is then used for managing it state.
	 * Starts at 0, which is expected to be assigned always to world.
	 */
	int32 NextUniqueControllerIdCount = 0;
	/**
	 * Assigns state to a Player/Ai.
	 */
	void AssignToSimulation(TObjectPtr<IUDControllerInterface> playerOrAi);
protected:
	/**
	 * Retrieves current GameState that is associated with the running level.
	 */
	TObjectPtr<AUDSkirmishGameState> GetCastGameState()
	{
		if (InternalCurrentGameState.IsNull())
		{
			InternalCurrentGameState = Cast<AUDSkirmishGameState>(GameState);
		}
		return InternalCurrentGameState;
	}
private:
	/**
	 * Saved pointer for the GameState to reduce amount of access casts.
	 * Access through the GetCastGameState(), this does not have to be initialized.
	 */
	TObjectPtr<AUDSkirmishGameState> InternalCurrentGameState;
};
