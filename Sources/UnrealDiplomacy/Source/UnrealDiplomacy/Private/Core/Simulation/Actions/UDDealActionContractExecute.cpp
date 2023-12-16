// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractExecute.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDDealActionContractReputationBonus.h"

bool UUDDealActionContractExecute::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isCorrectState = world->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Result;
	bool isOpen = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	bool isExecutable = world->Deals[data.DealId]->CanBeExecuted;

	return IUDActionInterface::CanExecute(action, world) && isModerator && isCorrectState && isOpen && isExecutable;
}

void UUDDealActionContractExecute::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	FUDDealData data(action.ValueParameters);
	// Execution is final action done over the deal
	world->Deals[data.DealId]->CanBeExecuted = false;
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Passed;
}

void UUDDealActionContractExecute::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	FUDDealData data(action.ValueParameters);
	// Revert is mostly trivial as it's done by previous actions in revert...
	world->Deals[data.DealId]->CanBeExecuted = true;
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}

TArray<FUDActionData> UUDDealActionContractExecute::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(parentAction.ValueParameters);
	return GetAllContractedActions(world, data.DealId);
}

TArray<FUDActionData> UUDDealActionContractExecute::GetAllContractedActions(TObjectPtr<UUDWorldState> world, int32 dealUniqueId) const
{
	TArray<FUDActionData> finalActionList;
	for (auto wrappedAction : world->Deals[dealUniqueId]->DealActionList)
	{
		if (!wrappedAction.WasSabotaged &&
			(
			wrappedAction.SelectedResult == EUDDealActionResult::Accepted ||
			wrappedAction.SelectedResult == EUDDealActionResult::Changed
			)
		)
		{
			finalActionList.Add(wrappedAction.Action);
		}
	}

	finalActionList.Add(FUDActionData(
		UUDDealActionContractReputationBonus::ActionTypeId,
		world->Deals[dealUniqueId]->OwnerUniqueId,
		{ dealUniqueId }
	));

	return finalActionList;
}