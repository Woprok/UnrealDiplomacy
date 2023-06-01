// Copyright Miroslav Valach

#include "Core/Simulation/UDActionHandlingInterface.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"

void IUDActionHandlingInterface::OnActionExecuted(FUDActionData executedAction)
{
	return;
}

void IUDActionHandlingInterface::SetSimulatedStateAccess(TObjectPtr<UUDWorldState> factionWorldState)
{
	return;
}