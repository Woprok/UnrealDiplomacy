// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"

bool UUDGameActionThroneUsurp::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// Throne must be empty.
		targetWorldState->ImperialThrone.Ruler == UUDWorldState::GaiaWorldStateId &&
		// Player should be the one who is currently playing his turn.
		actionData.InvokerPlayerId == targetWorldState->CurrentTurnPlayerId;
}

void UUDGameActionThroneUsurp::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Takeover the empty throne.
	targetWorldState->ImperialThrone.Ruler = actionData.InvokerPlayerId;
}

void UUDGameActionThroneUsurp::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Rollback to the empty throne.
	targetWorldState->ImperialThrone.Ruler = UUDWorldState::GaiaWorldStateId;
}