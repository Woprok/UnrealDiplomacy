// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionEndStateVote.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionEndStateVote::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isCorrectState = world->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::FinalVote;
	bool isOpen = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isModerator && isCorrectState && isOpen;
}

void UUDDealActionEndStateVote::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Advances the state to assembling.
	FUDDealData data(action.ValueParameters);

	int32 partCount = world->Deals[data.DealId]->Participants.Num();
	int32 okCount = world->Deals[data.DealId]->PositiveVotePlayerList.Num();
	// Deal Passed
	if (partCount == okCount)
	{
		world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::ResolutionOfPoints;
	}
	// Deal Failed
	else
	{
		world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Result;
		world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Vetoed;
	}
}

void UUDDealActionEndStateVote::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Reverts back to previous state.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::FinalVote;
}