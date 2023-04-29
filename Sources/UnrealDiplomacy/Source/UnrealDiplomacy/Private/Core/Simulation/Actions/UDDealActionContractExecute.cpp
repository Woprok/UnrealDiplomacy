// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractExecute.h"

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

bool UUDDealActionContractExecute::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionContractExecute::ConvertData(data);
		bool unresolved = UUDDealActionContractExecute::AreAllActionsPrepared(world, action.DealId);
		result = result && unresolved;
	}
	return result;
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

TArray<FUDActionData> UUDDealActionContractExecute::GetContinuations(FUDActionData& parentAction, TObjectPtr<UUDWorldState> world)
{
	TArray<FUDActionData> finalActionList;
	FUDDealData data = UUDDealActionContractExecute::ConvertData(parentAction);
	for (auto wrappedAction : world->Deals[action.DealId]->DealActionList)
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
	return finalActionList;
}