// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"

bool UUDSystemActionTurnEnd::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// Invoker must be current player.
		actionData.InvokerPlayerId == targetWorldState->CurrentTurnPlayerId;
}

void UUDSystemActionTurnEnd::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Find next player and update state.
	int32 nextPlayer = 0;
	for (auto& playerUniqueId : targetWorldState->PlayerOrder)
	{
		if (playerUniqueId > targetWorldState->CurrentTurnPlayerId)
		{
			nextPlayer = playerUniqueId;
			break;
		}
	}
	// Update turn player and counter.
	targetWorldState->CurrentTurnPlayerId = nextPlayer;
	targetWorldState->CurrentTurn += 1;
}

void UUDSystemActionTurnEnd::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Executing player is always the one who is ending the turn.
	// Force turn is used by server.
	int32 previousPlayer = actionData.InvokerPlayerId;
	// Rollback to the moment before turn end.
	targetWorldState->CurrentTurnPlayerId = previousPlayer;
	targetWorldState->CurrentTurn -= 1;
}