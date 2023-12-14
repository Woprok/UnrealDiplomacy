// Copyright Miroslav Valach

#include "Core/Simulation/UDStratagemOperationManager.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/Actions/UDGaiaActionFactionStratagemActivated.h"
#include "Core/Simulation/Actions/UDGaiaActionTileStratagemActivated.h"
#include "Core/Simulation/Actions/UDGaiaActionStratagemResourceCost.h"
#include "Core/Simulation/Modifiers/UDFactionModifierStratagemActivated.h"
#include "Core/Simulation/Modifiers/UDTileModifierStratagemActivated.h"
#include "Core/Simulation/Modifiers/UDFactionModifierStratagemShare.h"

void UUDStratagemOperationManager::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

void UUDStratagemOperationManager::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}

bool HasNotActivatedStratagem(const TArray<FUDModifierData>& modifiers, int32 searchedActionId, int32 position = 0)
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

bool UUDStratagemOperationManager::IsStratagem(const FUDActionPresentation& detail) const
{
	if (detail.Tags.Contains(UD_ACTION_TAG_STRATAGEM))
	{
		// We are interested only in stratagems.
		// If something is not a stratagem, then it will need another tag in future to handle ?
		return true;
	}

	// This action is not interesting for this manager and we can skip it.
	return false;
}

bool UUDStratagemOperationManager::HasStratagemFromOtherFaction(const TObjectPtr<UUDFactionState>& faction, int32 stratagemId) const
{
	const auto& modifiers = ModifierManager->GetAllFactionModifiers(faction, UUDFactionModifierStratagemShare::ModifierTypeId);

	for (const auto& mod : modifiers)
	{
		if (mod.ValueParameters[0] == stratagemId)
		{
			return true;
		}
	}

	return false;
}

bool UUDStratagemOperationManager::CanStratagemBeActivated(const TObjectPtr<UUDWorldState>& world, const FUDActionPresentation& detail, const FUDActionData& action) const
{
	bool canBeUsed = true;

	if (detail.Tags.Contains(UD_ACTION_TAG_STRATAGEM))
	{
		// Get reference to executor
		const TObjectPtr<UUDFactionState>& faction = world->Factions[action.InvokerFactionId];
		bool isFactionStratagem = faction->StratagemOptions.Contains(action.ActionTypeId);
		bool isSharedStratagem = HasStratagemFromOtherFaction(faction, action.ActionTypeId);
		canBeUsed == canBeUsed && (isFactionStratagem || isSharedStratagem);
	}

	// This is faction interaction, we will check if it was already used this turn by action invoker.
	// And we need to check it even if its tile interaction. 
	if (detail.Tags.Contains(UD_ACTION_TAG_FACTION_INTERACTION) || detail.Tags.Contains(UD_ACTION_TAG_TILE_INTERACTION))
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
		const auto tileModifiers = ModifierManager->GetAllTileModifiers(tile, UUDTileModifierStratagemActivated::ModifierTypeId);
		// This should be on first position in the value param arrays on modifier.
		canBeUsed = canBeUsed && HasNotActivatedStratagem(tileModifiers, detail.ActionId);
	}
		
	// At this point we should consider also non-tile interaction that leave modifier.
	// But that would mean that action is faction interaction that leaves modifier on tile or interacts with tile.
	// Thus we can safely ignore them as I can't think of any action that would be faction interaction and 
	// wanted to limit amount of time it can be used by different players on a single tile.

	// This will give us result, and we either stop execution or continue.
	return canBeUsed;
}

bool UUDStratagemOperationManager::HasCost(const TObjectPtr<UUDWorldState>& world, const FUDActionPresentation& detail, const FUDActionData& action) const
{
	bool canBeUsed = true;

	if (StratagemCosts.Contains(detail.ActionId))
	{
		const TObjectPtr<UUDFactionState>& faction = world->Factions[action.InvokerFactionId];
		canBeUsed = canBeUsed && CanPayActivationCost(faction, StratagemCosts[detail.ActionId].Costs);
	}
	else if (detail.Tags.Contains(UD_ACTION_TAG_RESOURCE_COST) || detail.Tags.Contains(UD_ACTION_TAG_STRATAGEM))
	{
		const TObjectPtr<UUDFactionState>& faction = world->Factions[action.InvokerFactionId];
		canBeUsed = canBeUsed && CanPayActivationCost(faction, DefaultStratagemCost.Costs);
	}

	// This will give us result, and we either stop execution or continue.
	return canBeUsed;
}

bool UUDStratagemOperationManager::CanPayActivationCost(const TObjectPtr<UUDFactionState>& faction, const TArray<FUDStratagemResourceCost>& costs) const
{
	bool canSpend = true;
	for (const auto& cost : costs)
	{
		canSpend = canSpend && ResourceManager->CanSpend(faction, cost.ResourceId, cost.ResourceCost);
	}
	return canSpend;
}

FUDActionData UUDStratagemOperationManager::GetActivationCostAction(int32 factionId, const TArray<FUDStratagemResourceCost>& costs) const
{
	TArray<int32> params = { factionId };
	for (const auto& cost : costs)
	{
		params.Add(cost.ResourceId);
		params.Add(cost.ResourceCost);
	}
	return FUDActionData(UUDGaiaActionStratagemResourceCost::ActionTypeId,
		UUDGlobalData::GaiaFactionId,
		params
	);
}

TArray<FUDActionData> UUDStratagemOperationManager::CreateStratagemConsequences(const FUDActionPresentation& detail, const FUDActionData& action) const
{
	TArray<FUDActionData> stratagemConsequences = { };

	// In this case we need to also include it, if player is using tile interaction.
	if (detail.Tags.Contains(UD_ACTION_TAG_FACTION_INTERACTION) || detail.Tags.Contains(UD_ACTION_TAG_TILE_INTERACTION))
	{
		FUDActionData factionStratagemActivatedAction(UUDGaiaActionFactionStratagemActivated::ActionTypeId,
			UUDGlobalData::GaiaFactionId,
			{ action.InvokerFactionId, action.ActionTypeId }
		);
		stratagemConsequences.Add(factionStratagemActivatedAction);
	}

	// This is tile interaction, we will check if it was already used this turn by anyone.
	if (detail.Tags.Contains(UD_ACTION_TAG_TILE_INTERACTION))
	{
		// This can safely assume that param 0 is X and param 1 is Y, see definition of TileInteractions.
		FUDActionData tileStratagemActivatedAction(UUDGaiaActionTileStratagemActivated::ActionTypeId,
			UUDGlobalData::GaiaFactionId,
			{ action.InvokerFactionId, action.ValueParameters[0], action.ValueParameters[1], action.ActionTypeId }
		);

		stratagemConsequences.Add(tileStratagemActivatedAction);
	}

	return stratagemConsequences;
}

TArray<FUDActionData> UUDStratagemOperationManager::CreateCostConsequences(const FUDActionPresentation& detail, const FUDActionData& action) const
{
	TArray<FUDActionData> stratagemConsequences = { };

	if (StratagemCosts.Contains(detail.ActionId))
	{
		stratagemConsequences.Add(GetActivationCostAction(action.InvokerFactionId, StratagemCosts[detail.ActionId].Costs));
	}
	else if (detail.Tags.Contains(UD_ACTION_TAG_RESOURCE_COST) || detail.Tags.Contains(UD_ACTION_TAG_STRATAGEM))
	{
		stratagemConsequences.Add(GetActivationCostAction(action.InvokerFactionId, DefaultStratagemCost.Costs));
	}

	return stratagemConsequences;
}