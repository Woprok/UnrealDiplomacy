// Copyright Miroslav Valach


#include "Core/Simulation/UDActionHandlingInterface.h"

void IUDActionHandlingInterface::OnActionExecuted(FUDActionData& executedAction)
{
	// Do nothing. Whoever inherits this should consider override for custom logic.
}