// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTake.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionTileTake::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	bool isTargetOwner = world->Map->GetTile(tile)->OwnerUniqueId == data.TargetId;
	return IUDActionInterface::CanExecute(action, world) && isTargetOwner;
}

void UUDGameActionTileTake::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Take the tile
	FUDGameDataTargetTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	world->Map->GetTile(tile)->OwnerUniqueId = action.InvokerFactionId;
}

void UUDGameActionTileTake::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Return the tile
	FUDGameDataTargetTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	world->Map->GetTile(tile)->OwnerUniqueId = data.TargetId;
}