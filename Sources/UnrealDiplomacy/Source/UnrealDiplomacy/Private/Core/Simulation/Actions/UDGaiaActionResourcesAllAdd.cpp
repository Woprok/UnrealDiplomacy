// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionResourcesAllAdd.h"
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

void UUDGaiaActionResourcesAllAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant all players defined amount of resources.
	//FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& faction : world->Factions)
	{
		ResourceManager->Gain(faction.Value, UUDGameResourceReputation::ResourceId, ReputationGain);
		ResourceManager->Gain(faction.Value, UUDGameResourceGold::ResourceId, GoldGain);
		ResourceManager->Gain(faction.Value, UUDGameResourceFood::ResourceId, FoodGain);
		ResourceManager->Gain(faction.Value, UUDGameResourceMaterials::ResourceId, MaterialsGain);
		ResourceManager->Gain(faction.Value, UUDGameResourceManpower::ResourceId, ManpowerGain);
		ResourceManager->Gain(faction.Value, UUDGameResourceLuxuries::ResourceId, LuxuriesGain);
	}
}

void UUDGaiaActionResourcesAllAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Take defined amount of resources from all players.
	//FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& faction : world->Factions)
	{
		ResourceManager->Lose(faction.Value, UUDGameResourceReputation::ResourceId, ReputationGain);
		ResourceManager->Lose(faction.Value, UUDGameResourceGold::ResourceId, GoldGain);
		ResourceManager->Lose(faction.Value, UUDGameResourceFood::ResourceId, FoodGain);
		ResourceManager->Lose(faction.Value, UUDGameResourceMaterials::ResourceId, MaterialsGain);
		ResourceManager->Lose(faction.Value, UUDGameResourceManpower::ResourceId, ManpowerGain);
		ResourceManager->Lose(faction.Value, UUDGameResourceLuxuries::ResourceId, LuxuriesGain);
	}
}

void UUDGaiaActionResourcesAllAdd::SetResourceManager(TObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}