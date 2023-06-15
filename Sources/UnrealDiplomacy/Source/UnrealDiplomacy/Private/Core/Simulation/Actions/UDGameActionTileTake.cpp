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

#define LOCTEXT_NAMESPACE "TileTake"
FUDActionPresentation UUDGameActionTileTake::GetPresentation() const
{
	if (GetId() != UUDGameActionTileTake::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("TileTake", "Take Tile")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_1,
			UD_ACTION_TAG_TILE_INTERACTION,
		}
	);

	return presentation;
}
#undef LOCTEXT_NAMESPACE