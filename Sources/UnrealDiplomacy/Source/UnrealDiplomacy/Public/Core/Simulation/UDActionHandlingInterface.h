// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionData.h"
#include "UDActionHandlingInterface.generated.h"

class AUDWorldSimulation;

/**
 * Interface for managing interaction between controllers (decision makers)
 * and simulation (executor)
 * This is required by UE compiler. This can't contain UNREALDIPLOMACY_API macro.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UUDActionHandlingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing interaction between controllers (decision makers)
 * and simulation (executor)
 */
class UNREALDIPLOMACY_API IUDActionHandlingInterface
{
	GENERATED_BODY()
public:
	/**
	 * Invoked everytime an action was decided by this controller.
	 */
	 DECLARE_DELEGATE_OneParam(ActionDecidedDelegate, FUDActionData&);
	 /**
	  * Binded to simulation, as this is a way for this controller to be informed about change.
	  */
	 virtual void OnActionExecuted(FUDActionData& executedAction);
	 /**
	  * Delegate ?
	  */
	 ActionDecidedDelegate OnActionDecidedDelegate;
};