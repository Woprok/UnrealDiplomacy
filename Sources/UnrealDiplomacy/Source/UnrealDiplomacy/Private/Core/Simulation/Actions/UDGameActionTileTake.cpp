// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTake.h"

bool UUDGameActionTileTake::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDTargetTileData data = UUDGameActionTileTake::ConvertData(data);
		FIntPoint tile(action.X, action.Y);
		bool isTargetOwner = world->Map->GetTile(tile)->OwnerUniqueId == action.TargetId;
		result = result && isTargetOwner;
	}
	return result;
}

void UUDGameActionTileTake::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Takeover the tile
	FUDTargetTileData data = UUDGameActionTileTake::ConvertData(data);
	FIntPoint tile(action.X, action.Y);
	world->Map->GetTile(tile)->OwnerUniqueId = action.InvokerPlayerId;

}

void UUDGameActionTileTake::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Revert ownership
	FUDTargetTileData data = UUDGameActionTileTake::ConvertData(data);
	FIntPoint tile(action.X, action.Y);
	world->Map->GetTile(tile)->OwnerUniqueId = action.TargetId;

}