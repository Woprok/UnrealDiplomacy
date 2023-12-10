// Copyright Miroslav Valach
// TODO proper mechanic for raid that is actually correct mathematically as /2 *2 might not work as intended
// TODO add backup for revert

#include "Core/Simulation/Actions/UDGameActionTileRaid.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"

bool UUDGameActionTileRaid::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	bool isNotOwner = world->Map->GetTile(tile)->OwnerUniqueId != action.InvokerFactionId;
	return IUDActionInterface::CanExecute(action, world) && isNotOwner;
}

void UUDGameActionTileRaid::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Raid is conducted.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);

	const TObjectPtr<UUDTileState>& editedTile = world->Map->GetTile(tile);

	int32 resourceType = editedTile->ResourceType;
	editedTile->ResourceStockpile /= 2;

	ResourceManager->Add(world->Factions[action.InvokerFactionId], resourceType, BaseGain);
}

void UUDGameActionTileRaid::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Raid is reverted.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);

	const TObjectPtr<UUDTileState>& editedTile = world->Map->GetTile(tile);

	int32 resourceType = editedTile->ResourceType;
	editedTile->ResourceStockpile *= 2;

	ResourceManager->Substract(world->Factions[action.InvokerFactionId], resourceType, BaseGain);
}

#define LOCTEXT_NAMESPACE "TileRaid"
FUDActionPresentation UUDGameActionTileRaid::GetPresentation() const
{
	if (GetId() != UUDGameActionTileRaid::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("TileRaid", "Tile Raid")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_2,
			UD_ACTION_TAG_TILE_INTERACTION,
			UD_ACTION_TAG_PARAMETER_TILE,
			UD_ACTION_TAG_DECISION_DIRECT,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("TileRaid",
		"Faction [{INVOKER}] raids province [{TILE}] owned by [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("TileRaid",
		"Faction [{INVOKER}] will raid province [{TILE}] owned by [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE

void UUDGameActionTileRaid::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}