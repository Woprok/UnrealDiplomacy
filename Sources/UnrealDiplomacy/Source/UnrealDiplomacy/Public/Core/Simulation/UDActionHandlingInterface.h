// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "UDActionHandlingInterface.generated.h"

/**
 * Invoked everytime an action was decided by this controller.
 */
DECLARE_DELEGATE_OneParam(ActionDecidedDelegate, FUDActionData);

/**
 * Interface for managing interaction between controllers (decision makers)
 * and simulation (executor). Used by AI.
 * This is required by UE compiler. This can't contain UNREALDIPLOMACY_API macro.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UUDActionHandlingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing interaction between controllers (decision makers)
 * and simulation (executor). Used by AI.
 */
class UNREALDIPLOMACY_API IUDActionHandlingInterface
{
	GENERATED_BODY()
public:
	 /**
	  * Binded to simulation, as this is a way for this controller to be informed about change.
	  * TODO register action executed as event/delegate to normalize how this interacts with GM & GS.
	  */
	 virtual void OnActionExecuted(FUDActionData executedAction)
	 {
		 // Default Interface call is empty.
	 }
	 /**
	  * Delegate ?
	  */
	 ActionDecidedDelegate OnActionDecidedDelegate;
	 /**
	  * Used to grant access to game state to owner of this interface.
	  * This access should be used only for reading the world state.
	  */
	 virtual void SetSimulatedStateAccess(TObjectPtr<UUDWorldState> personalWorldState)
	 {
		 // Default Interface call is empty.
	 }
};