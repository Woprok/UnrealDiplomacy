// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileBuildTradeGuild.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingTradeGuild.h"
#include "Core/Simulation/Resources/UDGameResourceGold.h"

bool UUDGameActionTileBuildTradeGuild::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);

	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingTradeGuild::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);

	bool isNotGaiaOwner = world->Map->GetTile(tile)->OwnerUniqueId != UUDGlobalData::GaiaFactionId;
	return IUDActionInterface::CanExecute(action, world) && isNotGaiaOwner;
}

void UUDGameActionTileBuildTradeGuild::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Build structure.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);
	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingTradeGuild::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);
	ModifierManager->CreateTileModifier(editedTile, modifierData);

	if (editedTile->ResourceType == UUDGameResourceGold::ResourceId)
	{
		editedTile->ResourceStockpile += (FlatResourceBonus + ExtraResourceBonus);
	}
	else
	{
		editedTile->ResourceStockpile += FlatResourceBonus;
	}
}

void UUDGameActionTileBuildTradeGuild::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Destroy structure.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const auto& editedTile = world->Map->GetTile(tile);
	ModifierManager->RemoveTileModifier(editedTile, UUDTileModifierBuildingTradeGuild::ModifierTypeId, action.UniqueId);

	if (editedTile->ResourceType == UUDGameResourceGold::ResourceId)
	{
		editedTile->ResourceStockpile -= (FlatResourceBonus + ExtraResourceBonus);
	}
	else
	{
		editedTile->ResourceStockpile -= FlatResourceBonus;
	}
}

void UUDGameActionTileBuildTradeGuild::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

#define LOCTEXT_NAMESPACE "TileBuildTradeGuild"
FUDActionPresentation UUDGameActionTileBuildTradeGuild::GetPresentation() const
{
	if (GetId() != UUDGameActionTileBuildTradeGuild::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("TileBuildTradeGuild", "Tile Build Trade Guild")).ToString();
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

	presentation.MessageContentFormat = FText(LOCTEXT("TileBuildTradeGuild",
		"Faction [{INVOKER}] builds Trade Guild on province [{TILE}] owned by [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("TileBuildTradeGuild",
		"Faction [{INVOKER}] will build Trade Guild on province [{TILE}] owned by [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE