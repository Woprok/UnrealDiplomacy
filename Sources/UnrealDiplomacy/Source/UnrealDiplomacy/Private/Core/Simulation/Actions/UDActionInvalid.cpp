// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDActionInvalid.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDActionInvalid::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	// Breaks the execution after logging it with default.
	bool isValid = false;
	return IUDActionInterface::CanExecute(action, world) && isValid;
}

int32 UUDActionInvalid::GetId() const
{
	return UUDGlobalData::InvalidActionId;
}