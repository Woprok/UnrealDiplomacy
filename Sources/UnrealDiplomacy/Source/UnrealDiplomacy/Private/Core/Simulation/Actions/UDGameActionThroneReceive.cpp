// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneReceive.h"

bool UUDGameActionThroneReceive::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	return IUDActionInterface::CanExecute(data, world) &&
		// Throne must be empty.
		world->ImperialThrone.Ruler == UUDWorldState::GaiaWorldStateId &&
		// Crown can be granted only be the world
		action.InvokerPlayerId == world->GaiaWorldStateId;
}

void UUDGameActionThroneReceive::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Takeover the empty throne.
	FUDTargetData data = UUDGameActionThroneReceive::ConvertData(data);
	world->ImperialThrone.Ruler = action.TargetId;
}

void UUDGameActionThroneReceive::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Rollback to the empty throne.
	world->ImperialThrone.Ruler = UUDWorldState::GaiaWorldStateId;
}