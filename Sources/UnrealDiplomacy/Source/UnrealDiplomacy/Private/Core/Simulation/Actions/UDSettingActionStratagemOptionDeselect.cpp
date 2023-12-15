// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSettingActionStratagemOptionDeselect.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDSettingActionStratagemOptionDeselect::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value
	FUDSettingDataValue data(action.ValueParameters);
	world->Factions[action.InvokerFactionId]->PickedStratagemOptions.Remove(data.Value);
	world->Factions[action.InvokerFactionId]->AccessibleStratagemOptions.Remove(data.Value);
}

void UUDSettingActionStratagemOptionDeselect::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to backup value
	FUDSettingDataValue data(action.ValueParameters);
	world->Factions[action.InvokerFactionId]->PickedStratagemOptions.Add(data.Value);
	world->Factions[action.InvokerFactionId]->AccessibleStratagemOptions.Add(data.Value);
}