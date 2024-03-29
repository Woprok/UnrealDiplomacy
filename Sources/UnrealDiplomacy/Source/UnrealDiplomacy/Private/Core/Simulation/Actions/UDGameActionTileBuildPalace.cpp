// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileBuildPalace.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingPalace.h"
#include "Core/Simulation/Resources/UDGameResourceReputation.h"

bool UUDGameActionTileBuildPalace::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);

	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingPalace::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);

	bool isNotGaiaOwner = world->Map->GetTile(tile)->OwnerUniqueId != UUDGlobalData::GaiaFactionId;
	return IUDActionInterface::CanExecute(action, world) && isNotGaiaOwner;
}

void UUDGameActionTileBuildPalace::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Build structure.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);
	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingPalace::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);
	ModifierManager->CreateTileModifier(editedTile, modifierData);

	if (editedTile->ResourceType == UUDGameResourceReputation::ResourceId)
	{
		editedTile->ResourceStockpile += (FlatResourceBonus + ExtraResourceBonus);
	}
	else
	{
		editedTile->ResourceStockpile += FlatResourceBonus;
	}
}

void UUDGameActionTileBuildPalace::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Destroy structure.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);
	ModifierManager->RemoveTileModifier(editedTile, UUDTileModifierBuildingPalace::ModifierTypeId, action.UniqueId);

	if (editedTile->ResourceType == UUDGameResourceReputation::ResourceId)
	{
		editedTile->ResourceStockpile -= (FlatResourceBonus + ExtraResourceBonus);
	}
	else
	{
		editedTile->ResourceStockpile -= FlatResourceBonus;
	}
}

void UUDGameActionTileBuildPalace::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

#define LOCTEXT_NAMESPACE "TileBuildPalace"
FUDActionPresentation UUDGameActionTileBuildPalace::GetPresentation() const
{
	if (GetId() != UUDGameActionTileBuildPalace::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("TileBuildPalace", "Tile Build Palace")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_3,
			UD_ACTION_TAG_TILE_INTERACTION,
			UD_ACTION_TAG_PARAMETER_TILE,
			UD_ACTION_TAG_DECISION_DIRECT,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("TileBuildPalace",
		"Faction [{INVOKER}] builds Palace on province [{TILE}] owned by [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("TileBuildPalace",
		"Faction [{INVOKER}] will build Palace on province [{TILE}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE