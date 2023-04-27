// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneReceive.h"

bool UUDGameActionThroneReceive::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// Throne must be empty.
		targetWorldState->ImperialThrone.Ruler == UUDWorldState::GaiaWorldStateId &&
		// Crown can be granted only be the world
		actionData.InvokerPlayerId == targetWorldState->GaiaWorldStateId;
}

void UUDGameActionThroneReceive::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Takeover the empty throne.
	FUDTargetData data = UUDGameActionThroneReceive::ConvertData(actionData);
	targetWorldState->ImperialThrone.Ruler = data.TargetId;
}

void UUDGameActionThroneReceive::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Rollback to the empty throne.
	targetWorldState->ImperialThrone.Ruler = UUDWorldState::GaiaWorldStateId;
}