// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionVoteNo.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionVoteNo::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isVoting = world->Deals[data.DealId]->PositiveVotePlayerList.Contains(action.InvokerFactionId);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalVote;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isVoting && isStateOpen && isResultOpen;
}
void UUDDealActionVoteNo::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Revert to no vote.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->PositiveVotePlayerList.Remove(action.InvokerFactionId);
	world->Deals[data.DealId]->NegativeVotePlayerList.Add(action.InvokerFactionId);
}
void UUDDealActionVoteNo::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Vote yes.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->PositiveVotePlayerList.Add(action.InvokerFactionId);
	world->Deals[data.DealId]->NegativeVotePlayerList.Remove(action.InvokerFactionId);
}