// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGiftReject.h"

void UUDGameActionGiftReject::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Request is removed from queue, without any effect being applied.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDGameActionGiftReject::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Request is returned to queue.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDGiftAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
}