// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileBuildFarm.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingFarm.h"

bool UUDGameActionTileBuildFarm::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);

	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingFarm::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);

	bool isNotGaiaOwner = world->Map->GetTile(tile)->OwnerUniqueId != UUDGlobalData::GaiaFactionId;
	return IUDActionInterface::CanExecute(action, world) && isNotGaiaOwner;
}

void UUDGameActionTileBuildFarm::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Build structure.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);
	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingFarm::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);
	ModifierManager->CreateTileModifier(editedTile, modifierData);
}

void UUDGameActionTileBuildFarm::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Destroy structure.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);
	ModifierManager->RemoveTileModifier(editedTile, UUDTileModifierBuildingFarm::ModifierTypeId, action.UniqueId);
}

void UUDGameActionTileBuildFarm::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

#define LOCTEXT_NAMESPACE "TileBuildFarm"
FUDActionPresentation UUDGameActionTileBuildFarm::GetPresentation() const
{
	if (GetId() != UUDGameActionTileBuildFarm::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("TileBuildFarm", "Tile Build Farm")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_1,
			UD_ACTION_TAG_TILE_INTERACTION,
			UD_ACTION_TAG_PARAMETER_TILE,
			UD_ACTION_TAG_DECISION_DIRECT,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("TileBuildFarm",
		"Faction [{INVOKER}] builds Farm on province [{TILE}] owned by [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("TileBuildFarm",
		"Faction [{INVOKER}] will build Farm on province [{TILE}] owned by [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE