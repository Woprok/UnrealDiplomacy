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
protected:
	/**
	 * Required to be called as last thing during game setup.
	 * This will invoke UUDStartGameAction that handles pre-first turn play.
	 */
	virtual void StartGame();
	/** 
	 * Final part of initialization of fields for the gamemode such as actors.
	 */
	virtual void PostInitializeComponents() override;
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
	 * Simulation that is responsible for maintaining and managing all interactions.
	 */
	TObjectPtr<AUDWorldSimulation> WorldSimulation;
private:
	/**
	 * Updates Simulation with known actions, that are necessary for Skirmish gameplay.
	 */
	virtual void LoadSkirmishAction();
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
};
