// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTake.h"

bool UUDGameActionTileTake::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDTargetTileData data = UUDGameActionTileTake::ConvertData(actionData);
		FIntPoint tile(data.X, data.Y);
		bool isTargetOwner = targetWorldState->Map->GetTile(tile)->OwnerUniqueId == data.TargetId;
		result = result && isTargetOwner;
	}
	return result;
}

void UUDGameActionTileTake::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Takeover the tile
	FUDTargetTileData data = UUDGameActionTileTake::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	targetWorldState->Map->GetTile(tile)->OwnerUniqueId = actionData.InvokerPlayerId;

}

void UUDGameActionTileTake::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Revert ownership
	FUDTargetTileData data = UUDGameActionTileTake::ConvertData(actionData);
	FIntPoint tile(data.X, data.Y);
	targetWorldState->Map->GetTile(tile)->OwnerUniqueId = data.TargetId;

}