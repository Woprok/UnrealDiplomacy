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
 */
UCLASS()
class UNREALDIPLOMACY_API AUDAIController : public AModularAIController, public IUDControllerInterface, public IUDActionHandlingInterface
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void SetControllerUniqueId(int32 uniqueControllerId) override;
	UFUNCTION()
	virtual int32 GetControllerUniqueId() override;
	UFUNCTION()
	virtual void SetControlledFactionId(int32 factionId) override;
	UFUNCTION()
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
	 */
	virtual void ProcessPlayableAction(const FUDActionData& executedAction);
	/**
	 * Enables handling of actions that were made outside of match.
	 * e.g. during pre-match and post-match.
	 */
	virtual void ProcessNonPlayableAction(const FUDActionData& executedAction);
	/**
	 * Enables handling of action that caused AI to start its turn.
	 * During this call AI is responsible for ending it's turn!
	 */
	virtual void ProcessInTurnPlay();
	/**
	 * Enables handling of actions that were made outside of current AI turn.
	 * e.g. interacting with another player or deal.
	 */
	virtual void ProcessOutTurnPlay();
private:
	/**
	 * Internal handle for dealing with out of turn actions to avoid cycle.
	 */
	void InTurnExecution();
	bool InTurnPlay = false;
	/**
	 * Internal handle for dealing with out of turn actions to avoid cycle.
	 */
	void OutTurnExecution();
	bool OutTurnPlay = false;
};