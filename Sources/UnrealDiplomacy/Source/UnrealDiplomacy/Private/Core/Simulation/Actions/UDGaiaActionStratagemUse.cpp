// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionStratagemUse.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/Resources/UDGameResourceReputation.h"
#include "Core/Simulation/Resources/UDGameResourceGold.h"
#include "Core/Simulation/Resources/UDGameResourceFood.h"
#include "Core/Simulation/Resources/UDGameResourceMaterials.h"
#include "Core/Simulation/Resources/UDGameResourceManpower.h"
#include "Core/Simulation/Resources/UDGameResourceLuxuries.h"

void UUDGaiaActionStratagemUse::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant all players defined amount of resources.
	//FUDGaiaDataAmount data(action.ValueParameters);
	//for (auto& tile : world->Map->Tiles)
	//{
	//	// Technically Gaia should not lose resources, but it has no effect on the game...
	//	const auto& faction = world->Factions[tile->OwnerUniqueId];
	//	for (const auto& upkeep : TileUpkeeps[tile->Type].Upkeeps)
	//	{
	//		ResourceManager->Substract(faction, upkeep.ResourceId, upkeep.ResourceUpkeep);
	//	}
	//}
}

void UUDGaiaActionStratagemUse::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	//// Take defined amount of resources from all players.
	////FUDGaiaDataAmount data(action.ValueParameters);
	//for (auto& tile : world->Map->Tiles)
	//{
	//	const auto& faction = world->Factions[tile->OwnerUniqueId];
	//	for (const auto& upkeep : TileUpkeeps[tile->Type].Upkeeps)
	//	{
	//		ResourceManager->Add(faction, upkeep.ResourceId, upkeep.ResourceUpkeep);
	//	}
	//}
}

void UUDGaiaActionStratagemUse::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}