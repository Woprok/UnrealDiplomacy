// Copyright Miroslav Valach
// TODO DELETE

#include "Core/Simulation/Actions/UDGameActionGiftReject.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

//bool UUDGameActionGiftReject::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
//{
//	FUDGameDataTargetResourceAmount data(action.ValueParameters);
//	bool isQueued = IsPendingTargetRequest(action, data.TargetId, world);
//	return UUDGameActionGift::CanExecute(action, world) && isQueued;
//}
//
//void UUDGameActionGiftReject::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
//{
//	IUDActionInterface::Execute(action, world);
//	// Request is removed from queue, without any effect being applied.
//	FUDGameDataTargetResourceAmount data(action.ValueParameters);
//	RemovePendingTargetRequest(action, data.TargetId, world);
//}
//
//void UUDGameActionGiftReject::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
//{
//	IUDActionInterface::Revert(action, world);
//	// Request is returned to queue.
//	FUDGameDataTargetResourceAmount data(action.ValueParameters);
//	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(action, UUDGameActionGift::ActionTypeId);
//	AddPendingTargetRequest(originalActionData, data.TargetId, world);
//}