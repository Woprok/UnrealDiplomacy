// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionStratagemResourceCost.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"

void UUDGaiaActionStratagemResourceCost::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant all players defined amount of resources.
	FUDGaiaDataTargetResourceAmount data(action.ValueParameters);
	const auto& faction = world->Factions[data.TargetId];
	for (auto& resourceCost : data.Costs)
	{
		ResourceManager->Substract(faction, resourceCost.ResourceId, resourceCost.Amount);
	}
}

void UUDGaiaActionStratagemResourceCost::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Take defined amount of resources from all players.
	FUDGaiaDataTargetResourceAmount data(action.ValueParameters);
	const auto& faction = world->Factions[data.TargetId];
	for (auto& resourceCost : data.Costs)
	{
		ResourceManager->Add(faction, resourceCost.ResourceId, resourceCost.Amount);
	}
}

FUDActionPresentation UUDGaiaActionStratagemResourceCost::GetPresentation() const
{
	FUDActionPresentation presentation = Super::GetPresentation();
	presentation.ActionId = GetId();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VERIFY_PARAMETER_MINIMUM,
		}
	);

	return presentation;
}

void UUDGaiaActionStratagemResourceCost::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}