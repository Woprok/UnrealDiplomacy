// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDSystemActionTurnEnd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isPlaying = action.InvokerFactionId == world->CurrentTurnPlayerId;
	return IUDActionInterface::CanExecute(action, world) && isPlaying;
}

void UUDSystemActionTurnEnd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Pass turn to next player.
	TurnAdvance(world);
}

void UUDSystemActionTurnEnd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Reverts previous player ending turn.
	RevertTurnAdvance(world, action.InvokerFactionId);
}

void UUDSystemActionTurnEnd::TurnAdvance(TObjectPtr<UUDWorldState> world)
{
	// Find turn player successor.
	int32 nextPlayerId = 0;
	for (auto& playerUniqueId : world->FactionOrder)
	{
		if (playerUniqueId > world->CurrentTurnPlayerId)
		{
			nextPlayerId = playerUniqueId;
			break;
		}
	}
	// Update turn player and counter.
	world->CurrentTurnPlayerId = nextPlayerId;
	world->CurrentTurn += 1;
}

void UUDSystemActionTurnEnd::RevertTurnAdvance(TObjectPtr<UUDWorldState> world, int32 previousPlayerId)
{
	world->CurrentTurnPlayerId = previousPlayerId;
	world->CurrentTurn -= 1;
}