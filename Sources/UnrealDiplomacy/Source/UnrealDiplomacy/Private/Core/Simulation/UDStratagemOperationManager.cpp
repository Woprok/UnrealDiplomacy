// Copyright Miroslav Valach

#include "Core/Simulation/UDStratagemOperationManager.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Actions/UDGaiaActionFactionStratagemActivated.h"
#include "Core/Simulation/Actions/UDGaiaActionTileStratagemActivated.h"
#include "Core/Simulation/Modifiers/UDFactionModifierStratagemActivated.h"
#include "Core/Simulation/Modifiers/UDTileModifierStratagemActivated.h"



bool UUDStratagemOperationManager::CanDoMagic(const TObjectPtr<UUDWorldState>& world, const TScriptInterface<IUDActionInterface>& executor, const FUDActionData& action)
{
	// we search for action.ActionTypeId in modifiers of type 
	// UUDGaiaActionFactionStratagemActivated or UUDGaiaActionTileStratagemActivated
	//ModifierManager->GetAllFactionModifiers(UUDFactionModifierStratagemActivated::ModifierTypeId);
	//ModifierManager->GetAllTileModifiers(UUDTileModifierStratagemActivated::ModifierTypeId);
	//if ()

	// if any tile has mod, or any faction has mod
	// then false
	// otherwise true


	// Neither Tile or Faction has the modifier that prevents stratagem use.
	return true;
}

void UUDStratagemOperationManager::DoMagic(const TObjectPtr<UUDWorldState>& world, const TScriptInterface<IUDActionInterface>& executor, const FUDActionData& action)
{
	FUDActionData data;
	FUDActionPresentation acd;

	TArray<FUDActionData> executeConsequence;

	// both can be activated at same time, e.g. raid ? limit once per phase on tile and only once per player
	// this is more restrictive
	if (acd.Tags.Contains(55)) // Single phase use for faction
	{
		data.InvokerFactionId;
		UUDGaiaActionFactionStratagemActivated::ActionTypeId;
		UUDGaiaActionTileStratagemActivated::ActionTypeId;
	}
	// this is less restrictive
	// problem is that we do not have position of the tile..., yep we made mistake
	// maybe we have ?
	// if action is TileINteraction it has tile first
	// if action is FactionInteraction it can have tile as second
	// thus if we are using tags we can just give it special order tag ?
	if (acd.Tags.Contains(55)) // Single phase use for tile
	{
		data.InvokerFactionId;
		UUDGaiaActionFactionStratagemActivated::ActionTypeId;
		UUDGaiaActionTileStratagemActivated::ActionTypeId;
	}


	// return executeConsequence;
}