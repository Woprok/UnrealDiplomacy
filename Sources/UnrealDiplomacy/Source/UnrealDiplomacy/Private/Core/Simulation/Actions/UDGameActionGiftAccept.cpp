// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGiftAccept.h"

void UUDGameActionGiftAccept::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Execute change based on data contained in confirm.
	FUDTargetValueData data = UUDGiftAction::ConvertData(data);
	world->Players[action.InvokerPlayerId]->ResourceGold -= action.Value;
	world->Players[action.TargetId]->ResourceGold += action.Value;
	// Remove request from queue.
	RemovePendingTargetRequest(data, action.TargetId, world);
}

void UUDGameActionGiftAccept::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDTargetValueData data = UUDGiftAction::ConvertData(data);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(data, UUDGiftAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, action.TargetId, world);
	// Revert change based on data that were used for confirmation..
	world->Players[action.InvokerPlayerId]->ResourceGold += action.Value;
	world->Players[action.TargetId]->ResourceGold -= action.Value;
}