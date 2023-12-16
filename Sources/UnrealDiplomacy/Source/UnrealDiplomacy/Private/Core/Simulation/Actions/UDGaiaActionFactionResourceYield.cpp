// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionFactionResourceYield.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Resources/UDGameResourceReputation.h"
#include "Core/Simulation/Resources/UDGameResourceGold.h"
#include "Core/Simulation/Resources/UDGameResourceFood.h"
#include "Core/Simulation/Resources/UDGameResourceMaterials.h"
#include "Core/Simulation/Resources/UDGameResourceManpower.h"
#include "Core/Simulation/Resources/UDGameResourceLuxuries.h"

UUDGaiaActionFactionResourceYield::UUDGaiaActionFactionResourceYield()
{
	StockpileGains.Add(UUDGameResourceReputation::ResourceId, 5);
	StockpileGains.Add(UUDGameResourceGold::ResourceId, 25);
	StockpileGains.Add(UUDGameResourceFood::ResourceId, 100);
	StockpileGains.Add(UUDGameResourceMaterials::ResourceId, 75);
	StockpileGains.Add(UUDGameResourceManpower::ResourceId, 50);
	StockpileGains.Add(UUDGameResourceLuxuries::ResourceId, 10);
}

void UUDGaiaActionFactionResourceYield::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant all players defined amount of resources.
	//FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& factionPair : world->Factions)
	{
		const auto& faction = factionPair.Value;
		for (const auto& resourcePair : StockpileGains)
		{
			ResourceManager->Gain(faction, resourcePair.Key, resourcePair.Value);
		}
	}
}

void UUDGaiaActionFactionResourceYield::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Take defined amount of resources from all players.
	//FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& factionPair : world->Factions)
	{
		const auto& faction = factionPair.Value;
		for (const auto& resourcePair : StockpileGains)
		{
			ResourceManager->Lose(faction, resourcePair.Key, resourcePair.Value);
		}
	}
}

void UUDGaiaActionFactionResourceYield::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}