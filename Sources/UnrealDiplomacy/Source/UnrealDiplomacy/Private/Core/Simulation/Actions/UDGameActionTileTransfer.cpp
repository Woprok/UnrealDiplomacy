// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionTileTransfer::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	bool isDifferentPlayer = action.InvokerFactionId != data.TargetId;
	bool isOwner = action.InvokerFactionId == world->Map->GetTile(tile)->OwnerUniqueId;
	return IUDActionInterface::CanExecute(action, world) && isDifferentPlayer && isOwner;
}

void UUDGameActionTileTransfer::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Execute change based on data. Invoker transfers to target.
	FUDGameDataTargetTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	world->Map->GetTile(tile)->OwnerUniqueId = data.TargetId;
}

void UUDGameActionTileTransfer::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revert change based on data. Invoker was one that gave away the tile.
	FUDGameDataTargetTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	world->Map->GetTile(tile)->OwnerUniqueId = action.InvokerFactionId;
}

#define LOCTEXT_NAMESPACE "TileTransfer"
FUDActionPresentation UUDGameActionTileTransfer::GetPresentation() const
{
	if (GetId() != UUDGameActionTileTransfer::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("TileTransfer", "Transfer Tile")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_2,
			UD_ACTION_TAG_FACTION_INTERACTION,
			UD_ACTION_TAG_PARAMETER_FACTION,
			UD_ACTION_TAG_PARAMETER_TILE,
			UD_ACTION_TAG_DECISION_OFFER,
			UD_ACTION_TAG_DECISION_REQUEST,
			UD_ACTION_TAG_DECISION_DEMAND,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("TileTransfer", 
		"Faction [{INVOKER}] transfers to faction [{TARGET}] province [{TILE}]."
	)).ToString();	
	presentation.DealContentFormat = FText(LOCTEXT("TileTransfer", 
		"Faction [{INVOKER}] will transfer control of province [{TILE}] to faction [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE