// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionResourcesAllAdd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDGaiaActionResourcesAllAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant all players defined amount of resources.
	FUDGaiaDataValue data = FUDGaiaDataValue(action.ValueParameters);
	for (auto& state : world->Players)
	{
		state.Value->ResourceGold += data.Value;
	}
}

void UUDGaiaActionResourcesAllAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Take defined amount of resources from all players.
	FUDGaiaDataValue data = FUDGaiaDataValue(action.ValueParameters);
	for (auto& state : world->Players)
	{
		state.Value->ResourceGold -= data.Value;
	}
}