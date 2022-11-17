// Copyright Miroslav Valach


#include "Core/Simulation/UDActionInterface.h"

bool IUDActionInterface::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return actionData.ActionTypeId == GetActionTypeId();
}

void IUDActionInterface::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
}

void IUDActionInterface::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
}

int32 IUDActionInterface::GetActionTypeId()
{
	return -1;
}

#pragma region UUDLogAction

void UUDLogAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): UUDLogAction was invoked by FUDActionData with id(%d), so it was logged due to UUDLogAction is id(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.ActionTypeId, ActionTypeId);
}

void UUDLogAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// LogAction is not revertable as there is no state change.
}

int32 UUDLogAction::GetActionTypeId()
{
	return ActionTypeId;
}

#pragma endregion

#pragma region UUDAddPlayerAction

void UUDAddPlayerAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): AddPlayer was invoked by new playerd with id(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId);
	targetWorldState->PlayerOrder.Add(UUDNationState::CreateState(actionData.InvokerPlayerId));
}

void UUDAddPlayerAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): AddPlayer was reverted by removing playerd with id(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId);
	auto& removedState = *targetWorldState->PlayerOrder.FindByPredicate(
			[&actionData](TObjectPtr<UUDNationState> state) { return state->PlayerUniqueId == actionData.InvokerPlayerId; }
	);
	targetWorldState->PlayerOrder.Remove(removedState);
}

int32 UUDAddPlayerAction::GetActionTypeId()
{
	return ActionTypeId;
}

#pragma endregion

#pragma region UUDEndTurnAction

bool UUDEndTurnAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		actionData.InvokerPlayerId == targetWorldState->CurrentTurnPlayerId;
}

void UUDEndTurnAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log, 
		TEXT("INSTANCE(%d): EndTurn was invoked by playerd with id(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId);
	// Search for next follower or jump to beginning.
	int32 nextPlayer = 0;

	for (auto& player : targetWorldState->PlayerOrder)
	{
		if (player->PlayerUniqueId > targetWorldState->CurrentTurnPlayerId)
		{
			nextPlayer = player->PlayerUniqueId;
			break;
		}
	}

	targetWorldState->CurrentTurnPlayerId = nextPlayer;

	// Update turn counter.
	targetWorldState->CurrentTurn += 1;
}

void UUDEndTurnAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Search for previous player is not necessary as it's always the one who invoked this action.
	// Technically this could backfire if server quits the player turn, but in that case the id passed should be
	// of the player that didn't finish his turn and was forced to give up.
	int32 previousPlayer = actionData.InvokerPlayerId;

	targetWorldState->CurrentTurnPlayerId = previousPlayer;

	// Revert turn counter.
	targetWorldState->CurrentTurn -= 1;
}

int32 UUDEndTurnAction::GetActionTypeId()
{
	return ActionTypeId;
}

#pragma endregion