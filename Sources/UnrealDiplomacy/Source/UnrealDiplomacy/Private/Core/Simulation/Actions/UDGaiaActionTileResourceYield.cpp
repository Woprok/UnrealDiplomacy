// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionTileResourceYield.h"
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

UUDGaiaActionTileResourceYield::UUDGaiaActionTileResourceYield()
{
	StockpilePercentages.Add(UUDGameResourceReputation::ResourceId, 1.00f);
	StockpilePercentages.Add(UUDGameResourceGold::ResourceId, 0.10f);
	StockpilePercentages.Add(UUDGameResourceFood::ResourceId, 0.25f);
	StockpilePercentages.Add(UUDGameResourceMaterials::ResourceId, 0.25f);
	StockpilePercentages.Add(UUDGameResourceManpower::ResourceId, 0.50f);
	StockpilePercentages.Add(UUDGameResourceLuxuries::ResourceId, 0.50f);
}

void UUDGaiaActionTileResourceYield::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant all players defined amount of resources.
	//FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& tile : world->Map->Tiles)
	{
		// Technically Gaia should not receive resources, but it has no effect on the game...
		const auto& faction = world->Factions[tile->OwnerUniqueId];
		const auto& perc = StockpilePercentages[tile->ResourceType];
		ResourceManager->Gain(faction, tile->ResourceType, tile->ResourceStockpile * perc);
	}
}

void UUDGaiaActionTileResourceYield::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Take defined amount of resources from all players.
	//FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& tile : world->Map->Tiles)
	{
		const auto& faction = world->Factions[tile->OwnerUniqueId];
		const auto& perc = StockpilePercentages[tile->ResourceType];
		ResourceManager->Lose(faction, tile->ResourceType, tile->ResourceStockpile * perc);
	}
}

void UUDGaiaActionTileResourceYield::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}