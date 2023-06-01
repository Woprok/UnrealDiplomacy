// Copyright Miroslav Valach
// TODO either expand this to player controllers or rename it to AIActionHandling

#pragma once

#include "CoreMinimal.h"
#include "UDActionHandlingInterface.generated.h"

// Forward Declarations

struct FUDActionData;
class UUDWorldState;

/**
 * Invoked everytime an action was decided by this controller.
 */
DECLARE_DELEGATE_OneParam(ActionDecidedDelegate, FUDActionData);

/**
 * Stumb for Action Interface.
 * Unreal Engine requires Interface to be first defined as empty stumb.
 * https://docs.unrealengine.com/5.2/en-US/interfaces-in-unreal-engine/
 */
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UUDActionHandlingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Enables interaction between the AI controllers and simulation owned by the Server.
 */
class UNREALDIPLOMACY_API IUDActionHandlingInterface
{
	GENERATED_BODY()
public:
	 /**
	  * Binded to simulation, as this is a way for this controller to be informed about change.
	  * TODO register action executed as event/delegate to normalize how this interacts with GM & GS.
	  */
	 virtual void OnActionExecuted(FUDActionData executedAction);
	 /**
	  * Used to grant access to game state to owner of this interface.
	  * This access should be used only for reading the world state.
	  */
	 virtual void SetSimulatedStateAccess(TObjectPtr<UUDWorldState> factionWorldState);
	 /**
	  * Handle for requesting action to be executed.
	  */
	 ActionDecidedDelegate OnActionDecidedDelegate;
};