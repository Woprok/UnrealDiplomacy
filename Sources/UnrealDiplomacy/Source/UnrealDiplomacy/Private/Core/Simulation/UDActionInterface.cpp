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

#pragma endregion

#pragma region UUDAddPlayerAction

void UUDAddPlayerAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): AddPlayer was invoked by new playerd with id(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId);
	targetWorldState->PlayerOrder.Add(actionData.InvokerPlayerId);
	targetWorldState->Players.Add(actionData.InvokerPlayerId, UUDNationState::CreateState(actionData.InvokerPlayerId));
}

void UUDAddPlayerAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): AddPlayer was reverted by removing playerd with id(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId);
	// Removing is trivial as long as we are using two separate lists.
	targetWorldState->PlayerOrder.Remove(actionData.InvokerPlayerId);
	targetWorldState->Players.Remove(actionData.InvokerPlayerId);
	// TODO remove this commented code during TODO cleanup
	//auto& removedState = *targetWorldState->PlayerOrder.FindByPredicate(
	//		[&actionData](TObjectPtr<UUDNationState> state) { return state->PlayerUniqueId == actionData.InvokerPlayerId; }
	//);
	//targetWorldState->PlayerOrder.Remove(removedState);
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

	for (auto& playerUniqueId : targetWorldState->PlayerOrder)
	{
		if (playerUniqueId > targetWorldState->CurrentTurnPlayerId)
		{
			nextPlayer = playerUniqueId;
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

#pragma endregion

#pragma region UUDGenerateIncomeAction
bool UUDGenerateIncomeAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		actionData.InvokerPlayerId == UUDWorldState::GaiaWorldStateId;
}

void UUDGenerateIncomeAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): Generate income invoked by player with id(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId);
	for (auto& state : targetWorldState->Players)
	{
		state.Value->ResourceGold += 100;
	}
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Generated income for each player."), targetWorldState->PerspectivePlayerId);
}

void UUDGenerateIncomeAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	for (auto& state : targetWorldState->Players)
	{
		state.Value->ResourceGold -= 100;
	}
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Removed last income for each player."), targetWorldState->PerspectivePlayerId);
}

#pragma endregion

#pragma region UUDUnconditionalGiftAction

bool UUDUnconditionalGiftAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		actionData.InvokerPlayerId != actionData.TargetPlayerId;
}

void UUDUnconditionalGiftAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): UnconditionalGift was invoked by playerd with id(%d) toward the player with id(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId, actionData.TargetPlayerId);
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold -= actionData.ValueParameter;
	targetWorldState->Players[actionData.TargetPlayerId]->ResourceGold += actionData.ValueParameter;
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): UnconditionalGift result: giver left (%d), receiver left (%d)."),
		targetWorldState->PerspectivePlayerId, 
		targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold, 
		targetWorldState->Players[actionData.TargetPlayerId]->ResourceGold);
}

void UUDUnconditionalGiftAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold += actionData.ValueParameter;
	targetWorldState->Players[actionData.TargetPlayerId]->ResourceGold -= actionData.ValueParameter;
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): UnconditionalGift undo result: giver left (%d), receiver left (%d)."),
		targetWorldState->PerspectivePlayerId,
		targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold,
		targetWorldState->Players[actionData.TargetPlayerId]->ResourceGold);
}

#pragma endregion