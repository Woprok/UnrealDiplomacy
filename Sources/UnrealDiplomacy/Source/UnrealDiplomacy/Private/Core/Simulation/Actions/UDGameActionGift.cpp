// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGift.h"

void UUDGameActionGift::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Queue new confirmable request.
	FUDTargetValueData data = UUDGameActionGift::ConvertData(actionData);
	AddPendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDGameActionGift::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove request from queue.
	FUDTargetValueData data = UUDGameActionGift::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}