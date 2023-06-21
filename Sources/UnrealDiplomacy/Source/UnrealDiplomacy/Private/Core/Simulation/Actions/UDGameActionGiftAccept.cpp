// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGiftAccept.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionGiftAccept::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetResourceAmount data(action.ValueParameters);
	bool isQueued = IsPendingTargetRequest(action, data.TargetId, world);
	return UUDGameActionGift::CanExecute(action, world) && isQueued;
}

void UUDGameActionGiftAccept::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Execute change based on data contained in confirm.
	FUDGameDataTargetResourceAmount data(action.ValueParameters);
	world->Factions[action.InvokerFactionId]->Resources[data.Resource] -= data.Amount;
	world->Factions[data.TargetId]->Resources[data.Resource] += data.Amount;
	// Remove request from queue.
	RemovePendingTargetRequest(action, data.TargetId, world);
}

void UUDGameActionGiftAccept::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDGameDataTargetResourceAmount data(action.ValueParameters);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(action, UUDGameActionGift::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, world);
	// Revert change based on data that were used for confirmation..
	world->Factions[action.InvokerFactionId]->Resources[data.Resource] += data.Amount;
	world->Factions[data.TargetId]->Resources[data.Resource] -= data.Amount;
}