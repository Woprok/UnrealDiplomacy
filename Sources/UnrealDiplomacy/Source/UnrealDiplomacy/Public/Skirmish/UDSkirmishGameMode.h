// Copyright Miroslav Valach
// TODO Reconnect feature
// -> Add session support for joining in progress game.
// --> Add functionality that allows server to disable the AI
// --> Add functionality that enables the AI, if player leaves the game after taking over the AI faction.
// -> Add UI support for joining in progress game.
// --> Observer UI for spectating.
// --> Takeover UI for taking over the AI controlled nation.
// --> Experimental in prgoress game faction creation ?
// -> Create command for taking over the specified AI faction.

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
	Match = 1,
	/**
	 * This instance is being killed and should not continue to do anything.
	 * Reason can be unexpected shut down of application or host leaving lobby.
	 */
	Closed = 2,
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
	/**
	 * Required for gracefull shutdown of the gamemode.
	 */
	virtual void OnCloseGameCommand();
#pragma endregion

#pragma region Controllers
protected:
	/**
	 * Handles AI taking over the player.
	 */
	virtual void OnPlayerLeaving(TObjectPtr<AUDSkirmishPlayerController> existingPlayer);
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
	 * RegisterAi that claims specified Faction.
	 */
	void RegisterSubstiteAi(int32 claimableFactionId);
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
	 * Remove reference on a specified player from all relevant places.
	 * It should be enough to remove only from PlayerControllers array.
	 * AI players do not need to be killed in this way as they can be halted by server, if needed.
	 */
	void DestroyPlayer(int32 controllerId);
protected:
	/**
	 * Assigns to controller new Id that will server as communication identificator.
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
	 * All controllers have unique id.
	 * Server controller is reserved in UUDGlobalData.
	 * Value of this starts based on FirstUSeableControllerId defined UUDGlobalData.
	 */
	UPROPERTY()
	int32 NextUniqueControllerId;
#pragma endregion
	UPROPERTY()
	EUDMatchState MatchState = EUDMatchState::Lobby;
};