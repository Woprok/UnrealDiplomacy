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
}

#pragma endregion

#pragma region UUDStartGameAction

void UUDStartGameAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): StartGameAction invoked."),
		targetWorldState->PerspectivePlayerId);
	IsGameStarted = true;
}

void UUDStartGameAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): StartGame was reverted."),
		targetWorldState->PerspectivePlayerId);
	IsGameStarted = false;
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

#pragma region UUDGiftAction

// TODO added CanExecute overloads that check if the action is still queued.
// Revert can not be executed, neither Execute if the action is in meantime reverted.

void GiftRemoveAction(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// requires to be found first, list should be always short.
	//int32 searchedId = actionData.UniqueId;
	//auto removedData = targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.FindByPredicate(
	//	[searchedId](FUDActionData& pendingData) { return pendingData.UniqueId == searchedId; }
	//);
	
	// Item is simply removed based on comparison
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): (%d) unresolved requests."),
		targetWorldState->PerspectivePlayerId, targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Num());
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Remove(actionData);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): (%d) unresolved requests."),
		targetWorldState->PerspectivePlayerId, targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Num());
}

void UUDGiftAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Unconfirmed request is added to queue.
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(actionData);
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):GiftAction(%d) assigned."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId);
}

void UUDGiftAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):GiftAction(%d) assign reverted."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId);

	// Unconfirmed request is removed from queue.
	GiftRemoveAction(actionData, targetWorldState);
}

void UUDConfirmGiftAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Action data is copied so we can use them instead of the original one.
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold -= actionData.ValueParameter;
	targetWorldState->Players[actionData.TargetPlayerId]->ResourceGold += actionData.ValueParameter;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):GiftAction(%d) applied. (s(%d) transfered (%d) to t(%d))"),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId, 
		actionData.ValueParameter,
		actionData.TargetPlayerId);

	// Request is removed from queue and it's effect is applied.
	GiftRemoveAction(actionData, targetWorldState);
}

void UUDConfirmGiftAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDActionData copy(actionData, UUDGiftAction::ActionTypeId);
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(copy);

	// Action data is copied so we can use them instead of the original one.
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold += actionData.ValueParameter;
	targetWorldState->Players[actionData.TargetPlayerId]->ResourceGold -= actionData.ValueParameter;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):GiftAction(%d) apply reverted. (s(%d) transfered (%d) to t(%d))"),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId,
		actionData.ValueParameter,
		actionData.TargetPlayerId);
}

void UUDRejectGiftAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):GiftAction(%d) removed."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId);

	// Request is removed from queue, without any effect being applied.
	GiftRemoveAction(actionData, targetWorldState);
}

void UUDRejectGiftAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Deny request is returned to queue.
	FUDActionData copy(actionData, UUDGiftAction::ActionTypeId);
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(copy);
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):GiftAction(%d) remove reverted."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId);
}

#pragma endregion

#pragma region UUDCreateWorldMapAction

void UUDCreateWorldMapAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Get reference to existing generator, this is expected set by the AUDWorldSimulation or whoever is executing actions.
	// Create empty state for the map to be filled in.
	targetWorldState->Map = UUDMapState::CreateState(actionData.ValueParameter, actionData.TileParameter.X, actionData.TileParameter.Y);
	// Generate world and replicate it to state, or just replicate if it exists.
	WorldGenerator->CreateAndDuplicate(targetWorldState->Map);
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):CreateWorldMap initialized."),
		targetWorldState->PerspectivePlayerId);
}

void UUDCreateWorldMapAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Map = nullptr;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):CreateWorldMap reverted."),
		targetWorldState->PerspectivePlayerId);
}

#pragma endregion

#pragma region UUDTakeTileAction

bool UUDTakeTileAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		targetWorldState->Map->Tiles[actionData.TileParameter.X][actionData.TileParameter.Y]->OwnerUniqueId == actionData.TargetPlayerId;
}

void UUDTakeTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Map->Tiles[actionData.TileParameter.X][actionData.TileParameter.Y]->OwnerUniqueId = actionData.InvokerPlayerId;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTakeTileAction tile(x=%d,y=%d) taken by %d."),
		targetWorldState->PerspectivePlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.InvokerPlayerId);
}

void UUDTakeTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Map->Tiles[actionData.TileParameter.X][actionData.TileParameter.Y]->OwnerUniqueId = actionData.TargetPlayerId;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTakeTileAction tile(x=%d,y=%d) taken by %d."),
		targetWorldState->PerspectivePlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.TargetPlayerId);
}

#pragma endregion