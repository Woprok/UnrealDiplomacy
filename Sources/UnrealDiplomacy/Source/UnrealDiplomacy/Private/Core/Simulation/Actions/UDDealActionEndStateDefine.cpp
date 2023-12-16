// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionEndStateDefine.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDDealActionContractCreate.h"

bool UUDDealActionEndStateDefine::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isCorrectState = world->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::DefinePoints;
	bool isOpen = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isModerator && isCorrectState && isOpen;
}

void UUDDealActionEndStateDefine::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Advances the state to assembling.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::FinalVote;
}

void UUDDealActionEndStateDefine::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Reverts back to previous state.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::DefinePoints;
}

TArray<FUDActionData> UUDDealActionEndStateDefine::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	TArray<FUDActionData> actions = { };
	FUDDealData data(parentAction.ValueParameters);
	int32 partCount = world->Deals[data.DealId]->Participants.Num();
	int32 okCount = world->Deals[data.DealId]->PositiveVotePlayerList.Num();
	// Deal Passed, thus create contract
	if (partCount == okCount)
	{
		actions.Add(FUDActionData(
			UUDDealActionContractCreate::ActionTypeId,
			world->Deals[data.DealId]->OwnerUniqueId,
			{ data.DealId }
		));
	}
	return actions;
}