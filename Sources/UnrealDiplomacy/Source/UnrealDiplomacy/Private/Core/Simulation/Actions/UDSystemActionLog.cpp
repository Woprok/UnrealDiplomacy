// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionLog.h"

void UUDSystemActionLog::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Execution is done by calling default implementation.
}

void UUDSystemActionLog::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Execution does not affect the state, so there is no revert.
}