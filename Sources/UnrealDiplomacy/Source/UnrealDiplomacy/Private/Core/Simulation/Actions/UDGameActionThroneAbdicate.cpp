// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneAbdicate.h"

bool UUDGameActionThroneAbdicate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	return IUDActionInterface::CanExecute(data, world) &&
		// Invoker must be current throne owner. No other conditions are requried for abdication.
		world->ImperialThrone.Ruler == action.InvokerPlayerId;
}

void UUDGameActionThroneAbdicate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Player is removed from the throne..
	world->ImperialThrone.Ruler = UUDWorldState::GaiaWorldStateId;
}

void UUDGameActionThroneAbdicate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Player is returned to throne.
	world->ImperialThrone.Ruler = action.InvokerPlayerId;
}