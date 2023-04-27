// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionTurnForceEnd.h"

bool UUDSystemActionTurnForceEnd::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// Invoker must be server.
		actionData.InvokerPlayerId == UUDWorldState::GaiaWorldStateId;
}

void UUDSystemActionTurnForceEnd::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Find next player and update state.
	FUDTargetData data = UUDForceEndTurnAction::ConvertData(actionData);
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

void UUDSystemActionTurnForceEnd::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Force turn is used by server and original player who was playing is stored in target.
	FUDTargetData data = UUDForceEndTurnAction::ConvertData(actionData);
	int32 previousPlayer = data.TargetId;
	// Rollback to the moment before turn end.
	targetWorldState->CurrentTurnPlayerId = previousPlayer;
	targetWorldState->CurrentTurn -= 1;
}