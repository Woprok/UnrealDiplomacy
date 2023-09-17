// Copyright Miroslav Valach
// TODO DELETE

#include "Core/Simulation/Actions/UDGameActionTileTransferAccept.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

//bool UUDGameActionTileTransferAccept::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
//{
//	FUDGameDataTargetTile data(action.ValueParameters);
//	bool isQueued = IsPendingTargetRequest(action, data.TargetId, world);
//	return UUDGameActionTileTransfer::CanExecute(action, world) && isQueued;
//}
//void UUDGameActionTileTransferAccept::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
//{
//	IUDActionInterface::Execute(action, world);
//	// Remove request from queue.
//	RemovePendingTargetRequest(action, data.TargetId, world);
//}
//
//void UUDGameActionTileTransferAccept::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
//{
//	IUDActionInterface::Revert(action, world);
//	// Confirmed request is returned to queue, but it has to be changed first.
//	AddPendingTargetRequest(originalActionData, data.TargetId, world);
//}