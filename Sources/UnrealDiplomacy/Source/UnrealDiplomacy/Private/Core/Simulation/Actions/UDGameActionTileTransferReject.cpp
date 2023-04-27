// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransferReject.h"

void UUDGameActionTileTransferReject::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Request is removed from queue, without any effect being applied.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDGameActionTileTransferReject::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Request is returned to queue.
	FUDTargetTileData data = UUDTransferTileAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDTransferTileAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
}