// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionStratagemActivatedReset.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Modifiers/UDTileModifierStratagemActivated.h"
#include "Core/Simulation/Modifiers/UDFactionModifierStratagemActivated.h"

void UUDGaiaActionStratagemActivatedReset::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Removes all modifiers from all factions and all tiles that are of StratagemUse type.
	for (const auto& tile : world->Map->Tiles)
	{
		ModifierManager->RemoveAllTileModifiersOfType(tile, UUDTileModifierStratagemActivated::ModifierTypeId);
	}
	for (const auto& faction : world->Factions)
	{
		ModifierManager->RemoveAllFactionModifiersOfType(faction.Value, UUDFactionModifierStratagemActivated::ModifierTypeId);
	}
}

void UUDGaiaActionStratagemActivatedReset::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// TODO add all modifiers back
}

void UUDGaiaActionStratagemActivatedReset::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}