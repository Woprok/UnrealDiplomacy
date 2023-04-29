// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionTurnForceEnd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDSystemActionTurnForceEnd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isGaia = action.InvokerPlayerId == UUDGlobalData::GaiaId;
	return IUDActionInterface::CanExecute(action, world) && isGaia;
}

void UUDSystemActionTurnForceEnd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Pass turn to next player.
	TurnAdvance(world);
}

void UUDSystemActionTurnForceEnd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Reverts previous player that was forced to end turn.
	FUDSystemDataTarget data(action.ValueParameters);
	RevertTurnAdvance(world, data.TargetId);
}