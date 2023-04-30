// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDefaultActionInvalid.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDefaultActionInvalid::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	// Breaks the execution after logging it with default.
	bool isValid = false;
	return IUDActionInterface::CanExecute(action, world) && isValid;
}

int32 UUDDefaultActionInvalid::GetId() const
{
	return UUDGlobalData::InvalidActionId;
}