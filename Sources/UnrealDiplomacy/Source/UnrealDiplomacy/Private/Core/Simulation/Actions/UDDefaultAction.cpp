// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDefaultAction.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDefaultAction::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	// Continues the execution after logging it with default.
	bool isValid = true;
	return IUDActionInterface::CanExecute(action, world) && isValid;
}