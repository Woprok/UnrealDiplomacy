// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransferAccept.h"

void UUDGameActionTileTransferAccept::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Execute change based on data contained in confirm.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	targetWorldState->Map->GetTile(tile)->OwnerUniqueId = data.TargetId;
	// Remove request from queue.
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDGameActionTileTransferAccept::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDGameActionTileTransferAccept::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
	// Revert change based on data that were used for confirmation..
	targetWorldState->Map->GetTile(tile)->OwnerUniqueId = actionData.InvokerPlayerId;
}