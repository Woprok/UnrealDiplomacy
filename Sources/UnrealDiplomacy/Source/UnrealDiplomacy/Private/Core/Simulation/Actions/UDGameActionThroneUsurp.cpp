// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"

bool UUDGameActionThroneUsurp::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	return IUDActionInterface::CanExecute(data, world) &&
		// Throne must be empty.
		world->ImperialThrone.Ruler == UUDWorldState::GaiaWorldStateId &&
		// Player should be the one who is currently playing his turn.
		action.InvokerPlayerId == world->CurrentTurnPlayerId;
}

void UUDGameActionThroneUsurp::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Takeover the empty throne.
	world->ImperialThrone.Ruler = action.InvokerPlayerId;
}

void UUDGameActionThroneUsurp::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Rollback to the empty throne.
	world->ImperialThrone.Ruler = UUDWorldState::GaiaWorldStateId;
}