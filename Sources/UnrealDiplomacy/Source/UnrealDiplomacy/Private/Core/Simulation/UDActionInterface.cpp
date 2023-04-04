// Copyright Miroslav Valach

// TODO LIST
// UUDUnconditionalGiftAction is missing condition (actionData.InvokerPlayerId != actionData.TargetPlayerId)
// TODO CanExecute overloads that check if the action is still queued in PendingRequests.
// TODO Create action class that has function RemovePendingTargetRequest as default implementation.
// TODO reverting confirm or reject does not result in same action, if the action would be modified, e.g.
// only part of value is accepted as gift the original action would not be restorable. In case of adding
// new option that makes partial accept, confirm for that action should be separate and it should extend parameter
// count with the original and changed value.
// TODO update modifiers to use predicates to minimize code
// TODO consider that action like responses or deal creation could be using sequence of actions e.g. composite
// this would allow them to do only one thing, while allowing additional effects to take place via consequent actions.
// TODO reevaluate UniqueId and SourceUniqueId based on composite actions and revert. Currently UniqueId is never used as
// it's available always the same way SourceUniqueId is and SourceUniqueId is universal identifier for response actions.
// This makes SDID more suitable in current code base as main Id during creating entities, while UniqueId has no role
// other than synchronization of Client and Server in batch.

#include "Core/Simulation/UDActionInterface.h"

bool IUDActionInterface::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Default Interface call is checking only ActionId and ParameterCount.
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Verifying Execution Action[Id(%d)==(%d), ValueCount(%d)==(%d)]."),
		targetWorldState->PerspectivePlayerId, actionData.ActionTypeId, GetActionTypeId(),
		actionData.ValueParameters.Num(), GetRequiredParametersCount());
	
	return actionData.ActionTypeId == GetActionTypeId() &&
		actionData.ValueParameters.Num() == GetRequiredParametersCount();
}	 

void IUDActionInterface::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Default Interface call is empty with a log.
	UE_LOG(
		LogTemp, 
		Log, 
		TEXT("INSTANCE(%d): Execuing Action[Id(%d), Invoker(%d), UID(%d), Source(%d), ValueCount(%d), TextLength(%d)]."),
		targetWorldState->PerspectivePlayerId, 
		actionData.ActionTypeId, 
		actionData.InvokerPlayerId,
		actionData.UniqueId,
		actionData.SourceUniqueId,
		actionData.ValueParameters.Num(),
		actionData.TextParameter.Len()
	);
}

void IUDActionInterface::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Default Interface call is empty with a log.
	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Execuing Action[Id(%d), Invoker(%d), UID(%d), Source(%d), ValueCount(%d), TextLength(%d)]."),
		targetWorldState->PerspectivePlayerId,
		actionData.ActionTypeId,
		actionData.InvokerPlayerId,
		actionData.UniqueId,
		actionData.SourceUniqueId,
		actionData.ValueParameters.Num(),
		actionData.TextParameter.Len()
	);
}

TArray<FUDActionData> IUDActionInterface::GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState)
{
	TArray<FUDActionData> emptyArray;
	emptyArray.Empty(0);
	return emptyArray;
}

bool IUDActionInterface::RequiresBackup()
{
	// Default interface call always returns false as it does no backup.
	return false;
}

void IUDActionInterface::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Default interface call is empty and should never be invoked by correctly defined action.
}

/**
 * Removes pending request associated with action and specified target.
 */
void RemovePendingTargetRequest(FUDActionData actionData, int32 targetId, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(
		LogTemp, 
		Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) requests. Deleting request(%d)"),
		targetWorldState->PerspectivePlayerId, 
		targetId,
		targetWorldState->Players[targetId]->PendingRequests.Num(),
		actionData.SourceUniqueId
	);
	
	// Item is removed based on the key.
	targetWorldState->Players[targetId]->PendingRequests.Remove(actionData.SourceUniqueId);

	UE_LOG(
		LogTemp, 
		Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) requests."),
		targetWorldState->PerspectivePlayerId,
		targetId,
		targetWorldState->Players[targetId]->PendingRequests.Num()
	);
}

/**
 * Adds pending request associated with action and specified target.
 */
void AddPendingTargetRequest(FUDActionData actionData, int32 targetId, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) requests. Adding request(%d)"),
		targetWorldState->PerspectivePlayerId,
		targetId,
		targetWorldState->Players[targetId]->PendingRequests.Num(),
		actionData.SourceUniqueId
	);

	// Item is added as key value pair.
	targetWorldState->Players[targetId]->PendingRequests.Add(actionData.SourceUniqueId, actionData);

	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) requests."),
		targetWorldState->PerspectivePlayerId,
		targetId,
		targetWorldState->Players[targetId]->PendingRequests.Num()
	);
}

#pragma region UUDLogAction

void UUDLogAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Execution is done by calling default implementation.
}

void UUDLogAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Execution does not affect the state, so there is no revert.
}

#pragma endregion

#pragma region UUDAddPlayerAction

void UUDAddPlayerAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Player is added to the state.
	targetWorldState->PlayerOrder.Add(actionData.InvokerPlayerId);
	targetWorldState->Players.Add(actionData.InvokerPlayerId, UUDNationState::CreateState(actionData.InvokerPlayerId));
}

void UUDAddPlayerAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Player is removed from the state.
	targetWorldState->PlayerOrder.Remove(actionData.InvokerPlayerId);
	targetWorldState->Players.Remove(actionData.InvokerPlayerId);
}

#pragma endregion

#pragma region UUDStartGameAction

bool UUDStartGameAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// State is checked.
		targetWorldState->WorldSimulationState == EUDWorldSimulationState::INITIALIZING;
}

void UUDStartGameAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// State is updated.
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::PLAYING;
}

void UUDStartGameAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// State is returned to previous value.
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::INITIALIZING;
}

#pragma endregion

#pragma region UUDEndGameAction

bool UUDEndGameAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// State is checked.
		targetWorldState->WorldSimulationState == EUDWorldSimulationState::PLAYING;
}

void UUDEndGameAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// State is updated.
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::FINISHING;
}

void UUDEndGameAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// State is returned to previous value.
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::PLAYING;
}

#pragma endregion

#pragma region UUDEndTurnAction

bool UUDEndTurnAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// Invoker must be current player.
		actionData.InvokerPlayerId == targetWorldState->CurrentTurnPlayerId;
}

void UUDEndTurnAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
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

void UUDEndTurnAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Executing player is always the one who is ending the turn.
	// Force turn is used by server.
	int32 previousPlayer = actionData.InvokerPlayerId;
	// Rollback to the moment before turn end.
	targetWorldState->CurrentTurnPlayerId = previousPlayer;
	targetWorldState->CurrentTurn -= 1;
}

bool UUDForceEndTurnAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// Invoker must be server.
		actionData.InvokerPlayerId == UUDWorldState::GaiaWorldStateId;
}

void UUDForceEndTurnAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
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

void UUDForceEndTurnAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Force turn is used by server and original player who was playing is stored in target.
	FUDTargetData data = UUDForceEndTurnAction::ConvertData(actionData);
	int32 previousPlayer = data.TargetId;
	// Rollback to the moment before turn end.
	targetWorldState->CurrentTurnPlayerId = previousPlayer;
	targetWorldState->CurrentTurn -= 1;
}

#pragma endregion

#pragma region UUDGenerateIncomeAction
bool UUDGenerateIncomeAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// Invoker must be server
		actionData.InvokerPlayerId == UUDWorldState::GaiaWorldStateId;
}

void UUDGenerateIncomeAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Grant all players defined amount of gold.
	FUDValueData data = UUDGenerateIncomeAction::ConvertData(actionData);
	for (auto& state : targetWorldState->Players)
	{
		state.Value->ResourceGold += data.Value;
	}
}

void UUDGenerateIncomeAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Take defined amount of gold from all players.
	FUDValueData data = UUDGenerateIncomeAction::ConvertData(actionData);
	for (auto& state : targetWorldState->Players)
	{
		state.Value->ResourceGold -= data.Value;
	}
}

#pragma endregion

#pragma region UUDUnconditionalGiftAction

bool UUDUnconditionalGiftAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState);
}

void UUDUnconditionalGiftAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Transfer resource to target.
	FUDTargetValueData data = UUDUnconditionalGiftAction::ConvertData(actionData);
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold -= data.Value;
	targetWorldState->Players[data.TargetId]->ResourceGold += data.Value;
}

void UUDUnconditionalGiftAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Transfer resource back from target.
	FUDTargetValueData data = UUDUnconditionalGiftAction::ConvertData(actionData);
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold += data.Value;
	targetWorldState->Players[data.TargetId]->ResourceGold -= data.Value;
}

#pragma endregion

#pragma region UUDGiftAction

void UUDGiftAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Queue new confirmable request.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	AddPendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDGiftAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove request from queue.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDConfirmGiftAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Execute change based on data contained in confirm.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold -= data.Value;
	targetWorldState->Players[data.TargetId]->ResourceGold += data.Value;
	// Remove request from queue.
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDConfirmGiftAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDGiftAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
	// Revert change based on data that were used for confirmation..
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold += data.Value;
	targetWorldState->Players[data.TargetId]->ResourceGold -= data.Value;
}

void UUDRejectGiftAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Request is removed from queue, without any effect being applied.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDRejectGiftAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Request is returned to queue.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDGiftAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
}

#pragma endregion

#pragma region UUDCreateWorldMapAction

void UUDCreateWorldMapAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Create new state and call generator to fill it with new world data.
	FUDMapSettingsData data = UUDCreateWorldMapAction::ConvertData(actionData);
	targetWorldState->Map = UUDMapState::CreateState(data.Seed, data.SizeX, data.SizeY);
	// Generated data are dumped into the provided state by the world generator automatically.
	WorldGenerator->CreateAndDuplicate(targetWorldState->Map);
}

void UUDCreateWorldMapAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Destroy map using GC.
	targetWorldState->Map = nullptr;
}

#pragma endregion

#pragma region UUDTakeTileAction

bool UUDTakeTileAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDTargetTileData data = UUDTakeTileAction::ConvertData(actionData);
		FIntPoint tile(data.X, data.Y);
		bool isTargetOwner = targetWorldState->Map->GetTile(tile)->OwnerUniqueId == data.TargetId;
		result = result && isTargetOwner;
	}
	return result;
}

void UUDTakeTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Takeover the tile
	FUDTargetTileData data = UUDTakeTileAction::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	targetWorldState->Map->GetTile(tile)->OwnerUniqueId = actionData.InvokerPlayerId;

}

void UUDTakeTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Revert ownership
	FUDTargetTileData data = UUDTakeTileAction::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	targetWorldState->Map->GetTile(tile)->OwnerUniqueId = data.TargetId;

}

#pragma endregion

#pragma region UUDExploitTileAction

const int UUDExploitTestValue = 100;

bool UUDExploitTileAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDTileValueData data = UUDExploitTileAction::ConvertData(actionData);
		FIntPoint tile(data.X, data.Y);
		bool isOwner = targetWorldState->Map->GetTile(tile)->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isRenting = false;

		for (auto& mod : ModifierManager->GetTileModifiers(
			targetWorldState->Map->GetTile(tile),
			UUDExploitTilePermissionModifier::ModifierTypeId)
			)
		{
			TObjectPtr<UUDExploitTilePermissionModifier> castedMod = Cast<UUDExploitTilePermissionModifier>(mod);
			if (castedMod->PlayerId == actionData.InvokerPlayerId)
			{
				isRenting = true;
				break;
			}
		}

		bool ownerOrRenting = isOwner || isRenting;
		result = result && ownerOrRenting;
	}
	return result;

}

void UUDExploitTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Apply exploitation
	FUDTileValueData data = UUDExploitTileAction::ConvertData(actionData);
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold += UUDExploitTestValue;
}

void UUDExploitTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove exploitation
	FUDTileValueData data = UUDExploitTileAction::ConvertData(actionData);
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold -= UUDExploitTestValue;
}

#pragma endregion

#pragma region UUDTransferTileAction

void UUDTransferTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Queue new confirmable request.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	AddPendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDTransferTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove request from queue.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDConfirmTransferTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Execute change based on data contained in confirm.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	targetWorldState->Map->GetTile(tile)->OwnerUniqueId = data.TargetId;
	// Remove request from queue.
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDConfirmTransferTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDTransferTileAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
	// Revert change based on data that were used for confirmation..
	targetWorldState->Map->GetTile(tile)->OwnerUniqueId = actionData.InvokerPlayerId;
}

void UUDRejectTransferTileAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Request is removed from queue, without any effect being applied.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDRejectTransferTileAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Request is returned to queue.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDTransferTileAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
}

#pragma endregion

#pragma region UUDGrantExploitTilePermissionAction

bool UUDGrantExploitTilePermissionAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDTargetTileData data = UUDGrantExploitTilePermissionAction::ConvertData(actionData);
		FIntPoint tile(data.X, data.Y);
		bool isInvokerOwner = targetWorldState->Map->GetTile(tile)->OwnerUniqueId == actionData.InvokerPlayerId;
		result = result && isInvokerOwner;
	}
	return result;
}

void UUDGrantExploitTilePermissionAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Apply modifier
	FUDTargetTileData data = UUDGrantExploitTilePermissionAction::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	ModifierManager->ApplyTileModifier(targetWorldState->Map->GetTile(tile),
		UUDExploitTilePermissionModifier::Create(actionData.SourceUniqueId, data.TargetId));
}

void UUDGrantExploitTilePermissionAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Removes modifier
	FUDTargetTileData data = UUDGrantExploitTilePermissionAction::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	ModifierManager->RemoveTileModifier(targetWorldState->Map->GetTile(tile), actionData.SourceUniqueId);
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
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
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
		result = result && canCreate;
	}
	return result;
}

void UUDCreateDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Creates new deal state with Id same as this action SourceUniqueId.
	targetWorldState->Deals.Add(actionData.SourceUniqueId,
		UUDDealState::CreateState(actionData.SourceUniqueId, actionData.InvokerPlayerId));
	targetWorldState->Deals[actionData.SourceUniqueId]->Participants.Add(actionData.InvokerPlayerId);
}

void UUDCreateDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Removes deal with key equal to SourceUniqueId of the original action.
	targetWorldState->Deals[actionData.SourceUniqueId]->Participants.Remove(actionData.InvokerPlayerId);
	targetWorldState->Deals.Remove(actionData.SourceUniqueId);
}

void UUDInviteParticipantDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Queue new confirmable request.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	AddPendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDInviteParticipantDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove request from queue.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDAcceptParticipationDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Execute change based on data contained in confirm.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Participants.Add(data.TargetId);
	// Remove request from queue.
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDAcceptParticipationDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDInviteParticipantDealAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
	// Revert change based on data that were used for confirmation.
	targetWorldState->Deals[data.DealId]->Participants.Remove(data.TargetId);
}

void UUDRejectParticipationDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Request is removed from queue, without any effect being applied.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
	// Block player future participation.
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Add(data.TargetId);
}

void UUDRejectParticipationDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Request is returned to queue.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDInviteParticipantDealAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
	// Enable blocked player participation.
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Remove(data.TargetId);
}

bool UUDKickParticipantDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealTargetData data = UUDKickParticipantDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isKickedPresent = targetWorldState->Deals[data.DealId]->Participants.Contains(data.TargetId);
		result = result && isStateOpen && isResultOpen && isModerator && isKickedPresent;
	}
	return result;
}

void UUDKickParticipantDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Remove invoker from participants
	FUDDealTargetData data = UUDKickParticipantDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Participants.Remove(data.TargetId);
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Add(data.TargetId);
}

void UUDKickParticipantDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Add invoker back to participants
	FUDDealTargetData data = UUDKickParticipantDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Remove(data.TargetId);
	targetWorldState->Deals[data.DealId]->Participants.Add(data.TargetId);
}

bool UUDLeaveParticipationDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDLeaveParticipationDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isLeaver = targetWorldState->Deals[data.DealId]->Participants.Contains(actionData.InvokerPlayerId);
		result = result && isStateOpen && isResultOpen && isLeaver;
	}
	return result;
}

void UUDLeaveParticipationDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Remove invoker from participants
	FUDDealData data = UUDLeaveParticipationDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Participants.Remove(actionData.InvokerPlayerId);
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Add(actionData.InvokerPlayerId);
}

void UUDLeaveParticipationDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Add invoker back to participants
	FUDDealData data = UUDLeaveParticipationDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Remove(actionData.InvokerPlayerId);
	targetWorldState->Deals[data.DealId]->Participants.Add(actionData.InvokerPlayerId);
}

#pragma endregion

#pragma region State and Result of Deal Action

bool UUDAdvanceStateAssemblingDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateAssemblingDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::CreatingDraft;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateAssemblingDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateAssemblingDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Assembling;
}
void UUDAdvanceStateAssemblingDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateAssemblingDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::CreatingDraft;
}

bool UUDAdvanceStateExtendingDraftDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateExtendingDraftDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Assembling;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateExtendingDraftDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateExtendingDraftDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::ExtendingDraft;
}
void UUDAdvanceStateExtendingDraftDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateExtendingDraftDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Assembling;
}

bool UUDAdvanceStateDemandsAndRequestsDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateDemandsAndRequestsDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::ExtendingDraft;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateDemandsAndRequestsDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateDemandsAndRequestsDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::DemandsAndRequests;
}
void UUDAdvanceStateDemandsAndRequestsDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateDemandsAndRequestsDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::ExtendingDraft;
}

bool UUDAdvanceStateBiddingDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateBiddingDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::DemandsAndRequests;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateBiddingDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateBiddingDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Bidding;
}
void UUDAdvanceStateBiddingDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateBiddingDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::DemandsAndRequests;
}

bool UUDAdvanceStateFinalizingDraftDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateFinalizingDraftDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Bidding;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateFinalizingDraftDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateFinalizingDraftDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::FinalizingDraft;
}
void UUDAdvanceStateFinalizingDraftDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateFinalizingDraftDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Bidding;
}

bool UUDAdvanceStateVoteDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateVoteDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::FinalizingDraft;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateVoteDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateVoteDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Vote;
}
void UUDAdvanceStateVoteDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateVoteDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::FinalizingDraft;
}

bool UUDAdvanceStateResolutionDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateResolutionDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Vote;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateResolutionDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateResolutionDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Resolution;
}
void UUDAdvanceStateResolutionDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateResolutionDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Vote;
}

bool UUDAdvanceStateResultDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateResultDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Resolution;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateResultDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateResultDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Result;
}
void UUDAdvanceStateResultDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateResultDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Resolution;
}

bool UUDAdvanceResultPassedDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceResultPassedDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceResultPassedDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceResultPassedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Passed;
}
void UUDAdvanceResultPassedDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceResultPassedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}

bool UUDAdvanceResultVetoedDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceResultVetoedDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceResultVetoedDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceResultVetoedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Vetoed;
}
void UUDAdvanceResultVetoedDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceResultVetoedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}

bool UUDAdvanceResultDisassembledDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceResultDisassembledDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceResultDisassembledDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDAdvanceResultDisassembledDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Disassembled;
}
void UUDAdvanceResultDisassembledDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDAdvanceResultDisassembledDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}

bool UUDAdvanceResultClosedDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceResultClosedDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceResultClosedDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDAdvanceResultClosedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Closed;
}
void UUDAdvanceResultClosedDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDAdvanceResultClosedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}

#pragma endregion

#pragma region Discussion Point

bool UUDAddDiscussionItemDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAddDiscussionItemDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDAddDiscussionItemDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Creates new point.
	FUDDealData data = UUDAddDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Add(actionData.SourceUniqueId, UUDDiscussionItem::CreateState(actionData.InvokerPlayerId));
	targetWorldState->Deals[data.DealId]->PrimaryPoints.Add(actionData.SourceUniqueId);
}
void UUDAddDiscussionItemDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Deletes created point that is currently located at the end.
	FUDDealData data = UUDAddDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Remove(actionData.SourceUniqueId);
	targetWorldState->Deals[data.DealId]->PrimaryPoints.Remove(actionData.SourceUniqueId);
}

bool UUDIgnoreDiscussionItemDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointData data = UUDIgnoreDiscussionItemDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isNotIgnored = targetWorldState->Deals[data.DealId]->Points[data.Point]->IsIgnored == false;
		result = result && isStateOpen && isResultOpen && isNotIgnored;
	}
	return result;
}
void UUDIgnoreDiscussionItemDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Mark point as ignored. Removing is not necessary.
	// TODO consider Id based indexing for Points...
	FUDDealPointData data = UUDIgnoreDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->IsIgnored = true;
}
void UUDIgnoreDiscussionItemDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Mark point as available for editing and use.
	FUDDealPointData data = UUDIgnoreDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->IsIgnored = false;
}

bool UUDSendMessageDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDSendMessageDealAction::ConvertData(actionData);
		bool isNotEmpty = actionData.TextParameter.Len() > 0;
		result = result && isNotEmpty;
	}
	return result;
}
void UUDSendMessageDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Add to history.
	FUDDealData data = UUDSendMessageDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->ChatHistory.Add(actionData.TextParameter);
}
void UUDSendMessageDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove last add message from the history.
	FUDDealData data = UUDSendMessageDealAction::ConvertData(actionData);
	int32 lastItem = targetWorldState->Deals[data.DealId]->ChatHistory.Num() - 1;
	targetWorldState->Deals[data.DealId]->ChatHistory.RemoveAt(lastItem);
}

bool UUDAddChildDiscussionItemDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointData data = UUDAddChildDiscussionItemDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDAddChildDiscussionItemDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Creates new sub-point for a specified Point with current SourceUniqueId.
	FUDDealPointData data = UUDAddChildDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Add(actionData.SourceUniqueId, UUDDiscussionItem::CreateState(actionData.InvokerPlayerId));
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Consequencies.Add(actionData.SourceUniqueId);
}
void UUDAddChildDiscussionItemDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Deletes created sub-point that is saved with this action SourceUniqueId.
	FUDDealPointData data = UUDAddChildDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Remove(actionData.SourceUniqueId);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Consequencies.Remove(actionData.SourceUniqueId);
}

#pragma endregion

#pragma region Deal Item Updates

bool UUDDefineActionDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDefineActionDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDefineActionDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointValueData newData = UUDDefineActionDealAction::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ActionId = newData.Value;
}
void UUDDefineActionDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old value.
	FUDDealPointValueData oldData = UUDDefineActionDealAction::ConvertBackupData(actionData);
	targetWorldState->Deals[oldData.DealId]->Points[oldData.Point]->ActionId = oldData.Value;
}
bool UUDDefineActionDealAction::RequiresBackup()
{
	return true;
}
void UUDDefineActionDealAction::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old action is backuped for future revert use.
	FUDDealPointValueData data = UUDDefineActionDealAction::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	actionData.BackupValueParameters.Add(targetWorldState->Deals[data.DealId]->Points[data.Point]->ActionId);
}

bool UUDDefinePointTypeDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDefinePointTypeDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDefinePointTypeDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointValueData newData = UUDDefinePointTypeDealAction::ConvertData(actionData);
	EUDPointType pointType = UUDDefinePointTypeDealAction::IntegerToPointType(newData.Value);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->Type = pointType;
}
void UUDDefinePointTypeDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);	
	// Change to old value.
	FUDDealPointValueData oldData = UUDDefinePointTypeDealAction::ConvertBackupData(actionData);
	EUDPointType pointType = UUDDefinePointTypeDealAction::IntegerToPointType(oldData.Value);
	targetWorldState->Deals[oldData.DealId]->Points[oldData.Point]->Type = pointType;
}
bool UUDDefinePointTypeDealAction::RequiresBackup()
{
	return true;
}
void UUDDefinePointTypeDealAction::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old action is backuped for future revert use.
	FUDDealPointValueData data = UUDDefinePointTypeDealAction::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	int32 pointType = UUDDefinePointTypeDealAction::PointTypeToInteger(targetWorldState->Deals[data.DealId]->Points[data.Point]->Type);
	actionData.BackupValueParameters.Add(pointType);
}


bool UUDAddInvokerDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDAddInvokerDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDAddInvokerDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDAddInvokerDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Add(data.Value);
}
void UUDAddInvokerDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDAddInvokerDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Remove(data.Value);
}

bool UUDRemoveInvokerDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDRemoveInvokerDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDRemoveInvokerDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDRemoveInvokerDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Remove(data.Value);
}
void UUDRemoveInvokerDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDRemoveInvokerDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Add(data.Value);
}

bool UUDAddTargetDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDAddTargetDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDAddTargetDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDAddTargetDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Add(data.Value);
}
void UUDAddTargetDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDAddTargetDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Remove(data.Value);
}

bool UUDRemoveTargetDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDRemoveTargetDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDRemoveTargetDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDRemoveTargetDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Remove(data.Value);
}
void UUDRemoveTargetDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDRemoveTargetDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Add(data.Value);
}


bool UUDReadyDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDReadyDealAction::ConvertData(actionData);
		result = result;
	}
	return result;
}
void UUDReadyDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDReadyDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Add(actionData.InvokerPlayerId);
}
void UUDReadyDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDReadyDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Remove(actionData.InvokerPlayerId);
}

bool UUDNotReadyDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDNotReadyDealAction::ConvertData(actionData);
		bool isReady = targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Contains(actionData.InvokerPlayerId);
		result = result && isReady;
	}
	return result;
}
void UUDNotReadyDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDNotReadyDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Remove(actionData.InvokerPlayerId);
}
void UUDNotReadyDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDNotReadyDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Add(actionData.InvokerPlayerId);
}
#pragma endregion