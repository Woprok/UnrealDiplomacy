// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGiftAccept.h"

void UUDGameActionGiftAccept::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Execute change based on data contained in confirm.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold -= data.Value;
	targetWorldState->Players[data.TargetId]->ResourceGold += data.Value;
	// Remove request from queue.
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDGameActionGiftAccept::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDTargetValueData data = UUDGiftAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDGiftAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
	// Revert change based on data that were used for confirmation..
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold += data.Value;
	targetWorldState->Players[data.TargetId]->ResourceGold -= data.Value;
}