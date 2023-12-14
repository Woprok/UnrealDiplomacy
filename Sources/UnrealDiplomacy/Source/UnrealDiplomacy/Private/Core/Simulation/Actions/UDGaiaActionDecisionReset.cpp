// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionDecisionReset.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/Resources/UDGameResourceReputation.h"
#include "Core/Simulation/Actions/UDDecisionActionDecline.h"

void UUDGaiaActionDecisionReset::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Removes all modifiers from all factions and all tiles that are of StratagemUse type.
	for (const auto& faction : world->Factions)
	{
		int32 factionPenaltyCount = 0;

		for (auto& decision : faction.Value->PendingDecisions)
		{
			faction.Value->PendingDecisions[decision.Key].Lifetime -= 1;
			if (faction.Value->PendingDecisions[decision.Key].Lifetime <= 0)
			{
				factionPenaltyCount++;
			}
		}

		ResourceManager->Substract(faction.Value, UUDGameResourceReputation::ResourceId, factionPenaltyCount * IgnoreReputationPenalty);
	}
}

void UUDGaiaActionDecisionReset::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// TODO add all decisions back
}

void UUDGaiaActionDecisionReset::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}

TArray<FUDActionData> UUDGaiaActionDecisionReset::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	TArray<FUDActionData> finalActionList;

	for (const auto& faction : world->Factions)
	{
		for (auto& decision : faction.Value->PendingDecisions)
		{
			if (faction.Value->PendingDecisions[decision.Key].Lifetime <= 0)
			{
				finalActionList.Add(
					FUDActionData(UUDDecisionActionDecline::ActionTypeId,
						faction.Key,
						{ decision.Key }
					)
				);
			}
		}
	}

	return finalActionList;
}