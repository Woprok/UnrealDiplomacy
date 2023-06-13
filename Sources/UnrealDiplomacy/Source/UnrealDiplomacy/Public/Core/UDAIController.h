// Copyright Miroslav Valach
// TODO implement and use AI Action Administrator instead of generic one.

#pragma once

#include "CoreMinimal.h"
#include "ModularAIController.h"
#include "UDControllerInterface.h"
#include "Simulation/UDActionHandlingInterface.h"
#include "UDAIController.generated.h"

// Forward Declarations

struct FUDActionData;
class UUDWorldState;
class UUDActionAdministrator;

/**
 * Common ancestor for all AI variations.
 * AI should never change Faction it can control once it was assigned.
 * Provides wrapper around action execution:
 * OnActionExecuted -> ProcessPlayableAction -> Process(PreGame|Intermezzo|InTurn|OutTurn)Play.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API AUDAIController : public AModularAIController, public IUDControllerInterface, public IUDActionHandlingInterface
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void SetControllerUniqueId(int32 uniqueControllerId) override;
	UFUNCTION(BlueprintPure)
	virtual int32 GetControllerUniqueId() override;
	UFUNCTION()
	virtual void SetControlledFactionId(int32 factionId) override;
	UFUNCTION(BlueprintPure)
	virtual int32 GetControlledFactionId() override;
public:
	/**
	 * Use Process functions to handle AI turns.
	 * Also avoid any potential circular dependecies between actions, that could crash the game.
	 */
	virtual void OnActionExecuted(FUDActionData executedAction) override;
	virtual void SetSimulatedStateAccess(TObjectPtr<UUDWorldState> factionWorldState) override;
protected:
	UPROPERTY()
	int32 UniqueControllerId;
	UPROPERTY()
	int32 ControlledFactionId;
	UPROPERTY()
	TWeakObjectPtr<UUDWorldState> FactionState = nullptr;
protected:
	// Action Administrator
	/**
	 * Creates new instance of the administrator.
	 */
	UFUNCTION()
	void InitializeAdministrator();
	/**
	 * Retrives administrator used by this controller.
	 */
	TObjectPtr<UUDActionAdministrator> GetAdministrator();
	/**
	 * Handles logic verfication and game rules access over the state owned by this controller.
	 */
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> ActionAdministrator = nullptr;
protected:
	// Action Execution
	/** 
	 * Enables action to be processed before futher execution.
	 * Do not invoke new action in this function.
	 * Use this only to update internal trackers!
	 */
	virtual void ProcessPlayableAction(const FUDActionData& executedAction);
	/**
	 * Enables handling of action that were executed before the game started.
	 * Generally this should be used for defining stratagems and faction name.
	 * Note: ensure that action for inits and settings are called only once.
	 */
	virtual void ProcessPreGamePlay();
	/**
	 * Enables handling of action that caused AI to start its turn.
	 * This should be almost always Intermezzo End.
	 * During this call AI is responsible for finishing it's turn!
	 * Executes if intermezzo was not executed.
	 */
	virtual void ProcessInTurnPlay();
	/**
	 * Enables handling of actions that were made after finishing current turn.
	 * e.g. interacting with another player or deal...
	 * This is called if all previous calls were not called.
	 */
	virtual void ProcessOutTurnPlay();
	/**
	 * Enables handling of actions that were made during the intermezzo.
	 * Generally used by Gaia AI. 
	 * Player AI should only update internal trackers and never execute actions!
	 * This is not suitable time to do it as world is being changing by Gaia.
	 * Executes if pregame was not executed and game is played.
	 */
	virtual void ProcessIntermezzoPlay();
private:
	/**
	 * Internal handle for dealing with in of turn actions to avoid cycle during single execution.
	 */
	void InTurnExecution();
	bool InTurnPlay = false;
	/**
	 * Internal handle for dealing with out of turn actions to avoid cycle during single execution.
	 */
	void OutTurnExecution();
	bool OutTurnPlay = false;
	/**
	 * Internal handle for dealing with intermezzo actions to avoid cycle during single execution.
	 */
	void IntermezzoExecution();
	bool IntermezzoPlay = false;
	/**
	 * Internal handle for dealing with pregame actions to avoid cycle during single execution.
	 */
	void PreGameExecution();
	bool PreGamePlay = false;
};