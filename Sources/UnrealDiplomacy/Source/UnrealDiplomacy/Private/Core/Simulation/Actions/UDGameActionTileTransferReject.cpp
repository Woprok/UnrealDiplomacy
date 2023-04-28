// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransferReject.h"

void UUDGameActionTileTransferReject::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Request is removed from queue, without any effect being applied.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(data);
	RemovePendingTargetRequest(data, action.TargetId, world);
}

void UUDGameActionTileTransferReject::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Request is returned to queue.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(data);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(data, UUDTransferTileAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, action.TargetId, world);
}