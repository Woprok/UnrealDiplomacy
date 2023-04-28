// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"

void UUDGameActionTileTransfer::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Queue new confirmable request.
	FUDTargetTileData data = UUDGameActionTileTransfer::ConvertData(data);
	AddPendingTargetRequest(data, action.TargetId, world);
}

void UUDGameActionTileTransfer::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Remove request from queue.
	FUDTargetTileData data = UUDGameActionTileTransfer::ConvertData(data);
	RemovePendingTargetRequest(data, action.TargetId, world);
}
