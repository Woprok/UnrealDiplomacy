// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionTileStratagemActivated.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Modifiers/UDTileModifierStratagemActivated.h"

void UUDGaiaActionTileStratagemActivated::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Add modifier for the action type activated on the tile.
	// Invoker & Target are same, so we are always using Invoker, in addition we have Tile & Action in params
	FUDGaiaDataTargetTileAction data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierStratagemActivated::ModifierTypeId, action.UniqueId,
		data.TargetId, data.TargetId, { data.ActionTypeId }
	);
	ModifierManager->CreateTileModifier(world->Map->GetTile(tile), modifierData);
}

void UUDGaiaActionTileStratagemActivated::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove modifier for the action type activated from tile.
	// We can use action id (this is not type id!) as we are just reverting... and this runs as normal action.
	FUDGaiaDataTargetTileAction data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	ModifierManager->RemoveTileModifier(world->Map->GetTile(tile), action.UniqueId);
}

void UUDGaiaActionTileStratagemActivated::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}