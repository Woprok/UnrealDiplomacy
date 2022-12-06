// Copyright Miroslav Valach


#include "Core/Simulation/UDActionHandlingInterface.h"

void IUDActionHandlingInterface::OnActionExecuted(FUDActionData& executedAction)
{
	// Do nothing. Whoever inherits this should consider override for custom logic.
}

void IUDActionHandlingInterface::SetSimulatedStateAccess(TObjectPtr<UUDWorldState> personalWorldState)
{
	// Do nithing. Requires to be overwritten.
}