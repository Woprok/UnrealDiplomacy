// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSettingActionStratagemOptionSelect.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDSettingActionStratagemOptionSelect::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value
	FUDSettingDataValue data(action.ValueParameters);
	world->Factions[action.InvokerFactionId]->StratagemOptions.Add(data.Value);
}

void UUDSettingActionStratagemOptionSelect::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to backup value
	FUDSettingDataValue data(action.ValueParameters);
	world->Factions[action.InvokerFactionId]->StratagemOptions.Remove(data.Value);
}