// Copyright Miroslav Valach


#include "Core/Simulation/UDActionInterface.h"

bool IUDActionInterface::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Default Interface call is pure check of GetActionTypeId equality.
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Default Can Execute is %d==%d."),
		targetWorldState->PerspectivePlayerId, actionData.ActionTypeId, GetActionTypeId());
	return actionData.ActionTypeId == GetActionTypeId();
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

bool UUDStartGameAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		targetWorldState->WorldSimulationState == EUDWorldSimulationState::INITIALIZING;
}

void UUDStartGameAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): StartGameAction invoked."),
		targetWorldState->PerspectivePlayerId);
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::PLAYING;
}

void UUDStartGameAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): StartGame was reverted."),
		targetWorldState->PerspectivePlayerId);
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::INITIALIZING;
}

#pragma endregion

#pragma region UUDEndGameAction

bool UUDEndGameAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		targetWorldState->WorldSimulationState == EUDWorldSimulationState::PLAYING;
}

void UUDEndGameAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): EndGameAction invoked."),
		targetWorldState->PerspectivePlayerId);
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::FINISHING;
}

void UUDEndGameAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): EndGame was reverted."),
		targetWorldState->PerspectivePlayerId);
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::PLAYING;
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
	// Item is simply removed based on comparison
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): (%d) unresolved requests."),
		targetWorldState->PerspectivePlayerId, targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Num());
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Remove(actionData.ParentUniqueId);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): (%d) unresolved requests."),
		targetWorldState->PerspectivePlayerId, targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Num());
}

void UUDGiftAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Unconfirmed request is added to queue.
	// This is using original Id of this action
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(actionData.ParentUniqueId, actionData);
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
	FUDActionData parent = FUDActionData::CreateParent(actionData, UUDGiftAction::ActionTypeId);
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(parent.ParentUniqueId, parent);

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
	FUDActionData parent = FUDActionData::CreateParent(actionData, UUDGiftAction::ActionTypeId);
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(parent.ParentUniqueId, parent);
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):GiftAction(%d) remove reverted."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId);
}

#pragma endregion

#pragma region UUDCreateWorldMapAction

void UUDCreateWorldMapAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Get reference to existing generator, this is expected set by the UDWorldSimulation or whoever is executing actions.
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
		targetWorldState->Map->GetTile(actionData.TileParameter)->OwnerUniqueId == actionData.TargetPlayerId;
}

void UUDTakeTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Map->GetTile(actionData.TileParameter)->OwnerUniqueId = actionData.InvokerPlayerId;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTakeTileAction tile(x=%d,y=%d) taken by %d."),
		targetWorldState->PerspectivePlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.InvokerPlayerId);
}

void UUDTakeTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Map->GetTile(actionData.TileParameter)->OwnerUniqueId = actionData.TargetPlayerId;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTakeTileAction tile(x=%d,y=%d) returned to %d."),
		targetWorldState->PerspectivePlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.TargetPlayerId);
}

#pragma endregion

#pragma region UUDExploitTileAction

const int UUDExploitTestValue = 100;

bool UUDExploitTileAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool validByModifier = false;
	for (auto& mod : ModifierManager->GetTileModifiers(targetWorldState->Map->GetTile(actionData.TileParameter),
		UUDExploitTilePermissionModifier::ModifierTypeId))
	{
		TObjectPtr<UUDExploitTilePermissionModifier> castedMod = Cast<UUDExploitTilePermissionModifier>(mod);
		if (castedMod->PlayerId == actionData.InvokerPlayerId)
		{
			validByModifier = true;
			break;
		}
	}
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		(
			targetWorldState->Map->GetTile(actionData.TileParameter)->OwnerUniqueId == actionData.InvokerPlayerId ||
			validByModifier
		);
}

void UUDExploitTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold += UUDExploitTestValue;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDExploitTileAction tile(x=%d,y=%d) player(%d) exploited for %d."),
		targetWorldState->PerspectivePlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.InvokerPlayerId,
		UUDExploitTestValue);
}

void UUDExploitTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold -= UUDExploitTestValue;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDExploitTileAction tile(x=%d,y=%d) player(%d) returned exploit of %d."),
		targetWorldState->PerspectivePlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.InvokerPlayerId,
		UUDExploitTestValue);
}

#pragma endregion


#pragma region UUDTransferTileAction

// TODO added CanExecute overloads that check if the action is still queued.
// Revert can not be executed, neither Execute if the action is in meantime reverted.

void TransferTileRemoveAction(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Item is simply removed based on comparison
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): (%d) unresolved requests."),
		targetWorldState->PerspectivePlayerId, targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Num());
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Remove(actionData.ParentUniqueId);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): (%d) unresolved requests."),
		targetWorldState->PerspectivePlayerId, targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Num());
}

void UUDTransferTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Unconfirmed request is added to queue.
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(actionData.ParentUniqueId, actionData);
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTransferTileAction(%d) assigned."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId);
}

void UUDTransferTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTransferTileAction(%d) assign reverted."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId);

	// Unconfirmed request is removed from queue.
	TransferTileRemoveAction(actionData, targetWorldState);
}

void UUDConfirmTransferTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Action data is copied so we can use them instead of the original one.
	targetWorldState->Map->GetTile(actionData.TileParameter)->OwnerUniqueId = actionData.TargetPlayerId;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTransferTileAction(%d) applied. (s(%d) transfered tile[%d, %d] to t(%d))"),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.TargetPlayerId);

	// Request is removed from queue and it's effect is applied.
	TransferTileRemoveAction(actionData, targetWorldState);
}

void UUDConfirmTransferTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDActionData parent = FUDActionData::CreateParent(actionData, UUDTransferTileAction::ActionTypeId);
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(parent.ParentUniqueId, parent);

	// Action data is copied so we can use them instead of the original one.
	targetWorldState->Map->GetTile(actionData.TileParameter)->OwnerUniqueId = actionData.InvokerPlayerId;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTransferTileAction(%d) apply reverted. (s(%d) transfered tile[%d, %d] to t(%d))"),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.TargetPlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.InvokerPlayerId);
}

void UUDRejectTransferTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTransferTileAction(%d) removed."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId);

	// Request is removed from queue, without any effect being applied.
	TransferTileRemoveAction(actionData, targetWorldState);
}

void UUDRejectTransferTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Deny request is returned to queue.
	FUDActionData parent = FUDActionData::CreateParent(actionData, UUDTransferTileAction::ActionTypeId);
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(parent.ParentUniqueId, parent);
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDTransferTileAction(%d) remove reverted."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId);
}

#pragma endregion

#pragma region UUDGrantExploitTilePermissionAction

bool UUDGrantExploitTilePermissionAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		targetWorldState->Map->GetTile(actionData.TileParameter)->OwnerUniqueId == actionData.InvokerPlayerId;
}

void UUDGrantExploitTilePermissionAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	ModifierManager->ApplyTileModifier(targetWorldState->Map->GetTile(actionData.TileParameter),
		UUDExploitTilePermissionModifier::Create(actionData.UniqueId, actionData.TargetPlayerId));

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDGrantExploitTilePermissionAction tile(x=%d,y=%d) can be exploited by player(%d)."),
		targetWorldState->PerspectivePlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.TargetPlayerId);
}

void UUDGrantExploitTilePermissionAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	ModifierManager->RemoveTileModifier(targetWorldState->Map->GetTile(actionData.TileParameter), 
		actionData.UniqueId);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDGrantExploitTilePermissionAction tile(x=%d,y=%d) can no longer be exploited by player(%d)."),
		targetWorldState->PerspectivePlayerId,
		actionData.TileParameter.X, actionData.TileParameter.Y,
		actionData.TargetPlayerId);
}

#pragma endregion

#pragma region UUDDealAction

/**
 * Deal can be created only if all other deals are closed.
 * TODO bugfix issues with mutliple active deals owned by single player?
 * Currently it prevents only from having multiple deals in opened state...
 */
bool UUDCreateDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool canCreate = true;
	for (auto deal : targetWorldState->Deals)
	{
		if (deal.Value->DealSimulationResult == EUDDealSimulationResult::Opened &&
			deal.Value->OwnerUniqueId == actionData.InvokerPlayerId)
		{
			canCreate = false;
		}
	}
	return IUDActionInterface::CanExecute(actionData, targetWorldState) && canCreate;
}

void UUDCreateDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): CreateDeal was invoked by playerId(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId);
	targetWorldState->Deals.Add(actionData.UniqueId, 
		UUDDealState::CreateState(actionData.UniqueId, actionData.InvokerPlayerId));
	// all consequent actions needs to use this Id
	targetWorldState->Deals[actionData.ParentUniqueId]->Participants.Add(actionData.InvokerPlayerId);
}

void UUDCreateDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): CreateDeal was reverted by playerId(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId);
	// Reversing is trivial as we just need to delete the last deal.
	targetWorldState->Deals[actionData.ParentUniqueId]->Participants.Remove(actionData.InvokerPlayerId);
	targetWorldState->Deals.Remove(actionData.ParentUniqueId);
}

void RemoveInviteParticipantDealAction(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): (%d) unresolved requests."),
		targetWorldState->PerspectivePlayerId, targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Num());

	// Item is simply removed based on comparison
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Remove(actionData.ParentUniqueId);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): (%d) unresolved requests."),
		targetWorldState->PerspectivePlayerId, targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Num());
}

void UUDInviteParticipantDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Unconfirmed request is added to queue.
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(actionData.ParentUniqueId, actionData);
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): InviteParticipantDeal was invoked by playerId(%d) to playerId(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId, actionData.TargetPlayerId);
}

void UUDInviteParticipantDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): InviteParticipantDeal was reverted by playerId(%d) to playerId(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.InvokerPlayerId, actionData.TargetPlayerId);

	// Unconfirmed request is removed from queue.
	RemoveInviteParticipantDealAction(actionData, targetWorldState);
}

void UUDAcceptParticipationDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Action data is copied so we can use them instead of the original one.
	targetWorldState->Deals[actionData.ParentUniqueId]->Participants.Add(actionData.TargetPlayerId);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDAcceptParticipationDealAction(%d) applied. (s(%d) invite accepted by t(%d))"),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId,
		actionData.TargetPlayerId);

	// Request is removed from queue and it's effect is applied.
	RemoveInviteParticipantDealAction(actionData, targetWorldState);
}

void UUDAcceptParticipationDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDActionData parent = FUDActionData::CreateParent(actionData, UUDInviteParticipantDealAction::ActionTypeId);
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(parent.ParentUniqueId, parent);

	// Action data is copied so we can use them instead of the original one.
	targetWorldState->Deals[actionData.ParentUniqueId]->Participants.Remove(actionData.TargetPlayerId);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDAcceptParticipationDealAction(%d) reverted. (s(%d) invite restored by t(%d))"),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId,
		actionData.TargetPlayerId);
}

void UUDRejectParticipationDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDRejectParticipationDealAction(%d) applied. (s(%d) invite deny by t(%d))"),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId,
		actionData.TargetPlayerId);

	// Request is removed from queue, this causes effect to be applied, e.g. player can't join anymore.
	targetWorldState->Deals[actionData.ParentUniqueId]->BlockedParticipants.Add(actionData.TargetPlayerId);
	RemoveInviteParticipantDealAction(actionData, targetWorldState);
}

void UUDRejectParticipationDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Deals[actionData.ParentUniqueId]->BlockedParticipants.Remove(actionData.TargetPlayerId);
	// Deny request is returned to queue.
	FUDActionData parent = FUDActionData::CreateParent(actionData, UUDInviteParticipantDealAction::ActionTypeId);
	targetWorldState->Players[actionData.TargetPlayerId]->PendingRequests.Add(parent.ParentUniqueId, parent);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDRejectParticipationDealAction(%d) reverted. (s(%d) invite restored by t(%d))"),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId,
		actionData.TargetPlayerId);
}

void UUDLeaveParticipationDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDLeaveParticipationDealAction(%d) left by playerId(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId);

	// Removed from participants.
	targetWorldState->Deals[actionData.ParentUniqueId]->Participants.Remove(actionData.TargetPlayerId);
	targetWorldState->Deals[actionData.ParentUniqueId]->BlockedParticipants.Add(actionData.TargetPlayerId);
}

void UUDLeaveParticipationDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Readded to participants.
	targetWorldState->Deals[actionData.ParentUniqueId]->BlockedParticipants.Remove(actionData.TargetPlayerId);
	targetWorldState->Deals[actionData.ParentUniqueId]->Participants.Add(actionData.TargetPlayerId);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDLeaveParticipationDealAction(%d) rejoined by playerId(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId);
}

bool UUDCloseDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) && 
		targetWorldState->Deals[actionData.ParentUniqueId]->DealSimulationResult == EUDDealSimulationResult::Opened &&
		targetWorldState->Deals[actionData.ParentUniqueId]->OwnerUniqueId == actionData.InvokerPlayerId;
}

void UUDCloseDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Deals[actionData.ParentUniqueId]->DealSimulationResult = EUDDealSimulationResult::Closed;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDCloseDealAction(%d) closed by playerId(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId);
}

void UUDCloseDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	targetWorldState->Deals[actionData.ParentUniqueId]->DealSimulationResult = EUDDealSimulationResult::Opened;

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d):UUDCloseDealAction(%d) reopened by playerId(%d)."),
		targetWorldState->PerspectivePlayerId, actionData.UniqueId,
		actionData.InvokerPlayerId);
}

#pragma endregion