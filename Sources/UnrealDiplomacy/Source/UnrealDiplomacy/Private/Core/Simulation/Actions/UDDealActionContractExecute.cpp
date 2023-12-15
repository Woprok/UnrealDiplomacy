// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractExecute.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDDealActionContractReputationBonus.h"

bool UUDDealActionContractExecute::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool areAllActionsResolved = UUDDealActionContractExecute::AreAllActionsPrepared(world, data.DealId);
	//bool dealIsNotExecuted = world->Deals[data.DealId]->;
	return IUDActionInterface::CanExecute(action, world) && areAllActionsResolved;//&& dealIsNotExecuted;
}
void UUDDealActionContractExecute::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);

	// Execution is empty as this action only invokes other.
}
void UUDDealActionContractExecute::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);

	// Execution is always reverted if all subactions were revoked.
}

TArray<FUDActionData> UUDDealActionContractExecute::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(parentAction.ValueParameters);
	return GetAllContractedActions(world, data.DealId);
}

bool UUDDealActionContractExecute::AreAllActionsPrepared(TObjectPtr<UUDWorldState> world, int32 dealUniqueId)
{
	for (auto actionWrapper : world->Deals[dealUniqueId]->DealActionList)
	{
		if (actionWrapper.SelectedResult < EUDDealActionResult::Accepted)
		{
			return false;
		}
	}
	return true;
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