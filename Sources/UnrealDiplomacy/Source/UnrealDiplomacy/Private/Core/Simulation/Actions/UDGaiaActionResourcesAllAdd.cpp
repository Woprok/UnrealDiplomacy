// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionResourcesAllAdd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDGaiaActionResourcesAllAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant all players defined amount of resources.
	FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& state : world->Factions)
	{
		state.Value->Resources[UD_RESOURCE_REPUTATION_ID] += data.Amount / 5;
		state.Value->Resources[UD_RESOURCE_GOLD_ID] += data.Amount;
		state.Value->Resources[UD_RESOURCE_FOOD_ID] += data.Amount * 5;
		state.Value->Resources[UD_RESOURCE_MATERIALS_ID] += data.Amount * 2.5;
		state.Value->Resources[UD_RESOURCE_LUXURIES_ID] += data.Amount / 10;
	}
}

void UUDGaiaActionResourcesAllAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Take defined amount of resources from all players.
	FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& state : world->Factions)
	{
		state.Value->Resources[UD_RESOURCE_REPUTATION_ID] -= data.Amount / 5;
		state.Value->Resources[UD_RESOURCE_GOLD_ID] -= data.Amount;
		state.Value->Resources[UD_RESOURCE_FOOD_ID] -= data.Amount * 5;
		state.Value->Resources[UD_RESOURCE_MATERIALS_ID] -= data.Amount * 2.5;
		state.Value->Resources[UD_RESOURCE_LUXURIES_ID] -= data.Amount / 10;
	}
}