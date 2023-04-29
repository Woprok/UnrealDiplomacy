// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransferReject.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionTileTransferReject::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetTile data(action.ValueParameters);
	bool isQueued = IsPendingTargetRequest(action, data.TargetId, world);
	return UUDGameActionTileTransferReject::CanExecute(action, world) && isQueued;
}
void UUDGameActionTileTransferReject::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Request is removed from queue, without any effect being applied.
	FUDGameDataTargetTile data(action.ValueParameters);
	RemovePendingTargetRequest(action, data.TargetId, world);
}

void UUDGameActionTileTransferReject::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Request is returned to queue.
	FUDGameDataTargetTile data(action.ValueParameters);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(action, UUDGameActionTileTransfer::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, world);
}