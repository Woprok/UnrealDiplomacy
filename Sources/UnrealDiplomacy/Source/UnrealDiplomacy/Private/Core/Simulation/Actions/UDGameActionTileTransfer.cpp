// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"

void UUDGameActionTileTransfer::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Queue new confirmable request.
	FUDTargetTileData data = UUDGameActionTileTransfer::ConvertData(actionData);
	AddPendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDGameActionTileTransfer::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove request from queue.
	FUDTargetTileData data = UUDGameActionTileTransfer::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}
