// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionFactionResourceDeficit.h"
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

void UUDGaiaActionFactionResourceDeficit::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant all players defined amount of resources.
	//FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& factionPair : world->Factions)
	{
		const auto& faction = factionPair.Value;
		const auto& factionResources = ResourceManager->GetAllCurrent(faction);
		int32 negativeCount = 0;
		// count negative
		for (const auto& resourcePair : factionResources)
		{
			if (resourcePair.Value < 0.0f)
			{
				negativeCount++;
			}
		}
		// penalize all non-negative resources
		for (const auto& resourcePair : factionResources)
		{
			if (resourcePair.Value > 0.0f)
			{
				int32 drainAmount = resourcePair.Value * negativeCount * PenaltyPerResource;
				drainAmount = FMath::Clamp(drainAmount, 0, resourcePair.Value);
				ResourceManager->Substract(faction, resourcePair.Key, drainAmount);
			}
		}
	}
}

void UUDGaiaActionFactionResourceDeficit::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Take defined amount of resources from all players.
	//FUDGaiaDataAmount data(action.ValueParameters);
	for (auto& factionPair : world->Factions)
	{
		// Todo correctly revert
	}
}

void UUDGaiaActionFactionResourceDeficit::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}