// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDGameMode.h"
#include "Core/UDControllerInterface.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include "Core/Simulation/UDActionData.h"
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
public:
	/**
	 * Process action that was invoked by Client. TODO as well for AI ?
	 */
	virtual void ProcessAction(FUDActionData& actionData);
	/**
	 * Required to be called as last thing during game setup.
	 * This will invoke UUDStartGameAction that handles pre-first turn play.
	 */
	virtual void StartGame(FUDActionData& actionData);
	/**
	 * Gathers and sends back requested historic data to specified controller.
	 * Parameters:
	 * controllerId
	 * firstKnwon - optional (0 = all) id of first already owned action.
	 */
	virtual void SendPartialHistoricData(int32 controllerId, int32 firstKnown);
protected:
	/**
	 * Handles decision how to propagate action that was successfully executed.
	 * GameMode handles passing notification and action to all Clients and AIs.
	 */
	UFUNCTION()
	virtual void ActionExecutionFinished(FUDActionData& actionData);
	/**
	 * Initializes all fields and prepares all objects for use.
	 */
	void Initialize();
	/**
	 * Registers listen to world simulation owned by this GameMode.
	 */
	UFUNCTION()
	virtual void RegisterAsListenerToWorldSimulation();
protected:
	/**
	 * Creates sufficient amount of Ai controllers.
	 */
	virtual void CreateAiPlayers(int32 count);
	/**
	 * Invokes call to register newly joined PlayerController for a game.
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/**
	 * Create Ai by initializing new AiController.
	 */
	virtual TWeakObjectPtr<AUDSkirmishAIController> CreateAi();
	/**
	 * Create Server player by initializing new AiController.
	 * This player acts as the world.
	 */
	virtual TWeakObjectPtr<AUDSkirmishGaiaAIController> CreateServerPlayer();
	/**
	 * Assigns state to Player and saves Player in PlayerControllers.
	 */
	void RegisterPlayer(APlayerController* NewPlayer);
	/**
	 * Assigns state to Gaia.
	 */
	void RegisterGaiaAi();
	/**
	 * Assigns state to Ai and saves Ai in AiControllers.
	 */
	void RegisterAi();
	/**
	 * Assigns state to a Player/Ai.
	 */
	void AssignToSimulation(TObjectPtr<IUDControllerInterface> controller, bool isPlayerOrAi);
protected:
	/**
	 * Lazy access to a WorldSimulation.
	 * Necessary to prevent early call of uninitialized fields.
	 */
	TWeakObjectPtr<AUDWorldSimulation> GetWorldSimulation()
	{
		if (!InternalWorldSimulation.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: New simulation required."));
			Initialize();
		}
		return InternalWorldSimulation;
	}
	/**
	 * Provides Id to agents, that we have full control over.
	 */
	int32 GetNextUniqueControllerId()
	{
		return NextUniqueControllerIdCount++;
	}
	/**
	 * Retrieves current GameState that is associated with the running level.
	 */
	TWeakObjectPtr<AUDSkirmishGameState> GetCastGameState()
	{
		if (!InternalCurrentGameState.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: New GameState required."));
			InternalCurrentGameState = Cast<AUDSkirmishGameState>(GameState);
		}
		return InternalCurrentGameState;
	}
private:
	/**
	 * Simulation that is responsible for maintaining and managing all interactions.
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDWorldSimulation> InternalWorldSimulation = nullptr;
	/**
	 * Instance of Gaia.
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDSkirmishGaiaAIController> GaiaController = nullptr;
	/**
	 * List of recognized bots.
	 */
	UPROPERTY()
	TArray<TWeakObjectPtr<AUDSkirmishAIController>> AiControllers;
	/**
	 * List of recognized players.
	 */
	UPROPERTY()
	TArray<TWeakObjectPtr<AUDSkirmishPlayerController>> PlayerControllers;
	/**
	 * Each controller posses unique Id, that is assigned to it via interface.
	 * This is then used for managing it state.
	 * Starts at 0, which is expected to be assigned always to world.
	 */
	UPROPERTY()
	int32 NextUniqueControllerIdCount = 0;
	/**
	 * Saved pointer for the GameState to reduce amount of access casts.
	 * Access through the GetCastGameState(), this does not have to be initialized.
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDSkirmishGameState> InternalCurrentGameState = nullptr;
};