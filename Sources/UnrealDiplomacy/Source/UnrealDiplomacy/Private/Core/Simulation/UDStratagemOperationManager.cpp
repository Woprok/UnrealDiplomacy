// Copyright Miroslav Valach

#include "Core/Simulation/UDStratagemOperationManager.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDGaiaActionFactionStratagemActivated.h"
#include "Core/Simulation/Actions/UDGaiaActionTileStratagemActivated.h"
#include "Core/Simulation/Modifiers/UDFactionModifierStratagemActivated.h"
#include "Core/Simulation/Modifiers/UDTileModifierStratagemActivated.h"

bool HasNotActivatedStratagem(const TArray<FUDModifierData>& modifiers, int32 searchedActionId, int32 position = 1)
{
	// Find
	const auto& found = modifiers.FindByPredicate(
		[&searchedActionId, &position](const FUDModifierData& item) { return item.ValueParameters[position] == searchedActionId; }
	);
	if (found)
	{
		return false;
	}
	return true;
}

bool UUDStratagemOperationManager::IsStratagemOrHasCost(const FUDActionPresentation& detail) const
{
	if (detail.Tags.Contains(UD_ACTION_TAG_STRATAGEM))
	{
		// We are interested only in stratagems.
		// If something is not a stratagem, then it will need another tag in future to handle ?
		return true;
	}

	// TODO COST!!!

	// This action is not interesting for this manager and we can skip it.
	return false;
}

bool UUDStratagemOperationManager::CanStratagemBeActivated(const TObjectPtr<UUDWorldState>& world, const FUDActionPresentation& detail, const FUDActionData& action) const
{
	bool canBeUsed = true;

	// This is faction interaction, we will check if it was already used this turn by action invoker.
	if (detail.Tags.Contains(UD_ACTION_TAG_FACTION_INTERACTION))
	{
		// Get reference to executor
		const TObjectPtr<UUDFactionState>& faction = world->Factions[action.InvokerFactionId];
		// Get all faction modifiers that are about stratagem activated.
		const auto factionModifiers = ModifierManager->GetAllFactionModifiers(faction, UUDFactionModifierStratagemActivated::ModifierTypeId);
		// This should be on first position in the value param arrays on modifier.
		canBeUsed = canBeUsed && HasNotActivatedStratagem(factionModifiers, detail.ActionId);
	}

	// This is tile interaction, we will check if it was already used this turn by anyone.
	if(detail.Tags.Contains(UD_ACTION_TAG_TILE_INTERACTION))
	{
		// Get reference, which is little harder for tile as we need to get coordinates, but these are always first two params.
		FIntPoint tilePosition(action.ValueParameters[0], action.ValueParameters[1]);
		const TObjectPtr<UUDTileState>& tile = world->Map->GetTile(tilePosition);
		// Get all tile modifiers that are about stratagem activated.
		const auto tileModifiers = ModifierManager->GetAllTileModifiers(tile, UUDFactionModifierStratagemActivated::ModifierTypeId);
		// This should be on first position in the value param arrays on modifier.
		canBeUsed = canBeUsed && HasNotActivatedStratagem(tileModifiers, detail.ActionId);
	}

	TSubclassOf<IUDActionInterface> ai;
	
	// At this point we should consider also non-tile interaction that leave modifier.
	// But that would mean that action is faction interaction that leaves modifier on tile or interacts with tile.
	// Thus we can safely ignore them as I can't think of any action that would be faction interaction and 
	// wanted to limit amount of time it can be used by different players on a single tile.


	// Finally we can check if this action has any cost associated with it.
	// If yes we will answer Y / N based on resources of the invoker.

	// TODO COST!!!

	// This will give us result, and we either stop execution or continue.
	return canBeUsed;
}

TArray<FUDActionData> UUDStratagemOperationManager::CreateConsequences(const TObjectPtr<UUDWorldState>& world, const FUDActionPresentation& detail, const FUDActionData& action) const
{
	TArray<FUDActionData> stratagemConsequences = { };

	// In this case we need to also include it, if player is using tile interaction.
	if (detail.Tags.Contains(UD_ACTION_TAG_FACTION_INTERACTION) || detail.Tags.Contains(UD_ACTION_TAG_TILE_INTERACTION))
	{
		FUDActionData factionStratagemActivatedAction(UUDGaiaActionFactionStratagemActivated::ActionTypeId,
			action.InvokerFactionId,
			{ action.ActionTypeId }
		);
		stratagemConsequences.Add(factionStratagemActivatedAction);
	}

	// This is tile interaction, we will check if it was already used this turn by anyone.
	if (detail.Tags.Contains(UD_ACTION_TAG_TILE_INTERACTION))
	{
		// This can safely assume that param 0 is X and param 1 is Y, see definition of TileInteractions.
		FUDActionData tileStratagemActivatedAction(UUDGaiaActionTileStratagemActivated::ActionTypeId,
			action.InvokerFactionId,
			{ action.ValueParameters[0], action.ValueParameters[1], action.ActionTypeId }
		);

		stratagemConsequences.Add(tileStratagemActivatedAction);
	}

	return stratagemConsequences;
}