// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionFactionStratagemActivated.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Modifiers/UDFactionModifierStratagemActivated.h"

void UUDGaiaActionFactionStratagemActivated::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Add modifier for the action type activated on the faction.
	// Invoker & Target are same, so we are always using Invoker, in addition we have Action in params
	FUDGaiaDataTargetAction data(action.ValueParameters);
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierStratagemActivated::ModifierTypeId, action.UniqueId,
		data.TargetId, data.TargetId, { data.ActionTypeId }
	);
	ModifierManager->CreateFactionModifier(world->Factions[data.TargetId], modifierData);
}

void UUDGaiaActionFactionStratagemActivated::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove modifier for the action type activated from faction.
	// We can use action id (this is not type id!) as we are just reverting... and this runs as normal action.
	// FUDGaiaDataAction data(action.ValueParameters);
	FUDGaiaDataTargetAction data(action.ValueParameters);
	ModifierManager->RemoveFactionModifier(world->Factions[data.TargetId], UUDFactionModifierStratagemActivated::ModifierTypeId, action.UniqueId);
}

void UUDGaiaActionFactionStratagemActivated::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}