// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionVoteYes.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionVoteYes::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isNotVoting = !world->Deals[data.DealId]->PositiveVotePlayerList.Contains(action.InvokerFactionId);
	return IUDActionInterface::CanExecute(action, world) && isNotVoting;
}

void UUDDealActionVoteYes::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Vote yes.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->PositiveVotePlayerList.Add(action.InvokerFactionId);
}

void UUDDealActionVoteYes::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revert to no vote.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->PositiveVotePlayerList.Remove(action.InvokerFactionId);
}