// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDGameMode.h"
#include "UDSkirmishGameMode.generated.h"

// Forward Declarations

struct FUDActionData;
struct FUDCommandData;
class AController;
class APlayerController;
class IUDControllerInterface;
class AUDSkirmishGaiaAIController;
class AUDSkirmishAIController;
class AUDSkirmishPlayerController;
class AUDSkirmishGameState;
class AUDWorldSimulation;

/**
 * Defines all available and supported commands for server.
 */
UENUM(BlueprintType)
enum class EUDMatchState : uint8
{
	/**
	 * Players that join will be assigned new faction.
	 */
	Lobby = 0,
	/**
	 * Players that join will be assigned AI observer faction.
	 */
	Match = 1
};

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishGameMode : public AUDGameMode
{
	GENERATED_BODY()
#pragma region Constructors & Initialization
public:
	AUDSkirmishGameMode();
protected:
	/**
	 * Initializes all fields and prepares all objects for use.
	 */
	void CreateWorldSimulation();
	/**
	 * Registers listen to world simulation owned by this GameMode.
	 */
	UFUNCTION()
	virtual void RegisterAsListenerToWorldSimulation();
	/**
	 * Lazy access to a WorldSimulation.
	 * Necessary to prevent early call of uninitialized fields.
	 */
	TWeakObjectPtr<AUDWorldSimulation> GetWorldSimulation();
	/**
	 * Retrieves current GameState that is associated with the running level.
	 */
	TWeakObjectPtr<AUDSkirmishGameState> GetCastGameState();
private:
	/**
	 * Simulation that is responsible for maintaining and managing all interactions.
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDWorldSimulation> InternalWorldSimulation = nullptr;
	/**
	 * Saved pointer for the GameState to reduce amount of access casts.
	 * Access through the GetCastGameState(), this does not have to be initialized.
	 */
	UPROPERTY()
	TWeakObjectPtr<AUDSkirmishGameState> InternalCurrentGameState = nullptr;
#pragma endregion

#pragma region Login & Logout
public:
	/**
	 * Invokes call to register newly joined PlayerController for a game.
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/**
	 * Called when a Controller with a PlayerState leaves the game or is destroyed.
	 */
	virtual void Logout(AController* Exiting) override;
#pragma endregion

#pragma region Actions & Commands
public:
	/**
	 * Process action that was invoked by Client. TODO as well for AI ?
	 */
	virtual void ProcessAction(FUDActionData& action);
	/**
	 * Process action that was invoked by Client.
	 */
	virtual void ProcessCommand(FUDCommandData& command);
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
	virtual void ActionExecutionFinished(FUDActionData& action);
	/**
	 * Required to be called as last thing during game setup.
	 * This will invoke UUDStartGameAction that handles pre-first turn play.
	 */
	virtual void OnStartGameCommand();
#pragma endregion

#pragma region Controllers
protected:
	/**
	 * Creates sufficient amount of Ai controllers.
	 */
	virtual void CreateAiPlayers(int32 count);
	/**
	 * Create Ai by initializing new AiController.
	 */
	virtual TWeakObjectPtr<AUDSkirmishAIController> CreateAiPlayer();
	/**
	 * Create Server player by initializing new AiController.
	 * This player acts as the world.
	 */
	virtual TWeakObjectPtr<AUDSkirmishGaiaAIController> CreateServerPlayer();
	/**
	 * Assigns state to Player and saves Player in PlayerControllers.
	 */
	void RegisterPlayer(TObjectPtr<AUDSkirmishPlayerController> NewPlayer);
	/**
	 * Assigns state to player and saves Player in PlayerControllers.
	 */
	void RegisterObserver(TObjectPtr<AUDSkirmishPlayerController> controller);
	/**
	 * Assigns state to Gaia.
	 * This should always happen first for server as it will use Gaia for all consequent actions.
	 */
	void RegisterGaiaAi();
	/**
	 * Assigns state to Ai and saves Ai in AiControllers.
	 */
	void RegisterAi();
	/**
	 * Assigns state to a Player/Ai.
	 */
	void AssignToSimulation(TScriptInterface<IUDControllerInterface>& controller, bool isPlayerOrAi);
protected
	/**
	 * Provides Id to agents, that we have full control over.
	 */
	void DefineUniqueControllerId(TScriptInterface<IUDControllerInterface>& controller);
private:
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
#pragma endregion

	UPROPERTY()
	EUDMatchState MatchState = EUDMatchState::Lobby;
};