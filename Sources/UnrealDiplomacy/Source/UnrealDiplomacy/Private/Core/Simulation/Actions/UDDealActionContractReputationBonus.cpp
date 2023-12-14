// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractReputationBonus.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/Resources/UDGameResourceReputation.h"

bool UUDDealActionContractReputationBonus::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	return IUDActionInterface::CanExecute(action, world);
}

void UUDDealActionContractReputationBonus::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// This will grant some reputation to all participants.
	FUDDealData data(action.ValueParameters);

	int32 failedPoints = 0;
	int32 fakedPoints = 0;
	int32 successfulPoints = 0;
	int32 participantCount = world->Deals[data.DealId]->Participants.Num();
	TSet<int32> active = { };

	for (auto wrappedAction : world->Deals[data.DealId]->DealActionList)
	{
		if (wrappedAction.SelectedResult == EUDDealActionResult::Accepted)
			successfulPoints++;
		else if (wrappedAction.SelectedResult == EUDDealActionResult::Changed)
			fakedPoints++;
		else
			failedPoints++;
		active.Add(wrappedAction.Action.InvokerFactionId);
	}
	int32 activeCount = active.Num();
	int32 pasiveCount = participantCount - active.Num();

	// For any sabotaged point they will losee up to 100%
	float sabotagePenalty = FMath::Min(1.0, failedPoints * PenaltyPerFailedFinalPoint + fakedPoints * PenaltyPerChangedFinalPoint);
	// Active add some small bonus, partially negating sabotage, capped at 100%
	float activeParticipantBonus = FMath::Min(1.0, participantCount * BonusPerDealActiveParticipant);
	// Size of the deal matters in small amount defined by passive cap.
	float passiveBonus = FMath::Min(PassiveCap, (successfulPoints + pasiveCount) * BonusPerDealPassivePoint);
	
	int32 amount = BaseDealReputation 
		- BaseDealReputation * sabotagePenalty
		+ BaseDealReputation * activeParticipantBonus
		+ BaseDealReputation * passiveBonus;

	for (const auto& factionId : world->Deals[data.DealId]->Participants)
	{
		const TObjectPtr<UUDFactionState>& faction = world->Factions[factionId];
		ResourceManager->Add(faction, UUDGameResourceReputation::ResourceId, amount);
	}
}

void UUDDealActionContractReputationBonus::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// This will revert the reputation granted.
	// TODO this revert.
}

void UUDDealActionContractReputationBonus::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}