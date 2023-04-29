// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionTileTransfer::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	bool isDifferentPlayer = action.InvokerPlayerId != data.TargetId;
	bool isOwner = action.InvokerPlayerId == world->Map->GetTile(tile)->OwnerUniqueId;
	return IUDActionInterface::CanExecute(action, world) && isDifferentPlayer && isOwner;
}

void UUDGameActionTileTransfer::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Queue new confirmable request.
	FUDGameDataTargetTile data(action.ValueParameters);
	AddPendingTargetRequest(action, data.TargetId, world);
}

void UUDGameActionTileTransfer::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDGameDataTargetTile data(action.ValueParameters);
	RemovePendingTargetRequest(action, data.TargetId, world);
}