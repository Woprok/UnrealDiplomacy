// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileRaid.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingFortress.h"

bool UUDGameActionTileRaid::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const TObjectPtr<UUDTileState>& targetTile = world->Map->GetTile(tile);
	bool isNotOwner = targetTile->OwnerUniqueId != action.InvokerFactionId;
	bool doesNotHaveFort = ModifierManager->GetAllTileModifiers(targetTile, UUDTileModifierBuildingFortress::ModifierTypeId).Num() <= 0;
	bool hasResourcesToRaid = targetTile->ResourceStockpile > MinimumForRaid;
	return IUDActionInterface::CanExecute(action, world) && isNotOwner && doesNotHaveFort && hasResourcesToRaid;
}

void UUDGameActionTileRaid::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Raid is conducted.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const TObjectPtr<UUDTileState>& editedTile = world->Map->GetTile(tile);

	int32 maxGain = FMath::Max(MinBaseGain, editedTile->ResourceStockpile * BaseGainPercentage);
	int32 maxDevastation = FMath::Max(MinBaseDevastation, editedTile->ResourceStockpile * BaseDevastationPercentage);

	editedTile->ResourceStockpile = FMath::Max(0, editedTile->ResourceStockpile - maxDevastation);

	ResourceManager->Add(world->Factions[action.InvokerFactionId], editedTile->ResourceType, maxGain);
}

void UUDGameActionTileRaid::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Raid is reverted.
	FUDGameDataTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	const TObjectPtr<UUDTileState>& editedTile = world->Map->GetTile(tile);

	// TODO proper revert code

	ResourceManager->Substract(world->Factions[action.InvokerFactionId], editedTile->ResourceType, MinBaseGain);
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

void UUDGameActionTileRaid::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

void UUDGameActionTileRaid::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}