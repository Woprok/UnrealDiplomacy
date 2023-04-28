// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGift.h"

void UUDGameActionGift::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Queue new confirmable request.
	FUDTargetValueData data = UUDGameActionGift::ConvertData(data);
	AddPendingTargetRequest(data, action.TargetId, world);
}

void UUDGameActionGift::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Remove request from queue.
	FUDTargetValueData data = UUDGameActionGift::ConvertData(data);
	RemovePendingTargetRequest(data, action.TargetId, world);
}