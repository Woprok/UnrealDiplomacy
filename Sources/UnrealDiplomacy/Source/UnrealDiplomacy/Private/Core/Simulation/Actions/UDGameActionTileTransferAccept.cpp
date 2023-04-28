// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransferAccept.h"

void UUDGameActionTileTransferAccept::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Execute change based on data contained in confirm.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(data);
	FIntPoint tile(action.X, action.Y);
	world->Map->GetTile(tile)->OwnerUniqueId = action.TargetId;
	// Remove request from queue.
	RemovePendingTargetRequest(data, action.TargetId, world);
}

void UUDGameActionTileTransferAccept::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(data);
	FIntPoint tile(action.X, action.Y);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(data, UUDGameActionTileTransferAccept::ActionTypeId);
	AddPendingTargetRequest(originalActionData, action.TargetId, world);
	// Revert change based on data that were used for confirmation..
	world->Map->GetTile(tile)->OwnerUniqueId = action.InvokerPlayerId;
}