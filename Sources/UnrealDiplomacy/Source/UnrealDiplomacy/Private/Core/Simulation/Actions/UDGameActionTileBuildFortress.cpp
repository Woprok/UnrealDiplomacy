// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileBuildFortress.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingFortress.h"
#include "Core/Simulation/Resources/UDGameResourceManpower.h"

bool UUDGameActionTileBuildFortress::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);

	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingFortress::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);

	bool isNotGaiaOwner = world->Map->GetTile(tile)->OwnerUniqueId != UUDGlobalData::GaiaFactionId;
	return IUDActionInterface::CanExecute(action, world) && isNotGaiaOwner;
}

void UUDGameActionTileBuildFortress::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Build structure.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);
	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingFortress::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);
	ModifierManager->CreateTileModifier(editedTile, modifierData);

	if (editedTile->ResourceType == UUDGameResourceManpower::ResourceId)
	{
		editedTile->ResourceStockpile += (FlatResourceBonus + ExtraResourceBonus);
	}
	else
	{
		editedTile->ResourceStockpile += FlatResourceBonus;
	}
}

void UUDGameActionTileBuildFortress::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Destroy structure.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);
	ModifierManager->RemoveTileModifier(editedTile, UUDTileModifierBuildingFortress::ModifierTypeId, action.UniqueId);

	if (editedTile->ResourceType == UUDGameResourceManpower::ResourceId)
	{
		editedTile->ResourceStockpile -= (FlatResourceBonus + ExtraResourceBonus);
	}
	else
	{
		editedTile->ResourceStockpile -= FlatResourceBonus;
	}
}

void UUDGameActionTileBuildFortress::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

#define LOCTEXT_NAMESPACE "TileBuildFortress"
FUDActionPresentation UUDGameActionTileBuildFortress::GetPresentation() const
{
	if (GetId() != UUDGameActionTileBuildFortress::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("TileBuildFortress", "Tile Build Fortress")).ToString();
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

	presentation.MessageContentFormat = FText(LOCTEXT("TileBuildFortress",
		"Faction [{INVOKER}] builds Fortress on province [{TILE}] owned by [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("TileBuildFortress",
		"Faction [{INVOKER}] will build Fortress on province [{TILE}] owned by [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE