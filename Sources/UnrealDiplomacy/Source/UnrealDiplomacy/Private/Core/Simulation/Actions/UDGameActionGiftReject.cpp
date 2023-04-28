// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGiftReject.h"

void UUDGameActionGiftReject::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Request is removed from queue, without any effect being applied.
	FUDTargetValueData data = UUDGiftAction::ConvertData(data);
	RemovePendingTargetRequest(data, action.TargetId, world);
}

void UUDGameActionGiftReject::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Request is returned to queue.
	FUDTargetValueData data = UUDGiftAction::ConvertData(data);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(data, UUDGiftAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, action.TargetId, world);
}