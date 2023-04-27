// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractExecute.h"

bool UUDDealActionContractExecute::AreAllActionsPrepared(TObjectPtr<UUDWorldState> targetWorldState, int32 dealUniqueId)
{
	for (auto actionWrapper : targetWorldState->Deals[dealUniqueId]->DealActionList)
	{
		if (actionWrapper.SelectedResult < EUDDealActionResult::Accepted)
		{
			return false;
		}
	}
	return true;
}

bool UUDDealActionContractExecute::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionContractExecute::ConvertData(actionData);
		bool unresolved = UUDDealActionContractExecute::AreAllActionsPrepared(targetWorldState, data.DealId);
		result = result && unresolved;
	}
	return result;
}
void UUDDealActionContractExecute::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Execution is empty as this action only invokes other.
}
void UUDDealActionContractExecute::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Execution is always reverted if all subactions were revoked.
}

TArray<FUDActionData> UUDDealActionContractExecute::GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState)
{
	TArray<FUDActionData> finalActionList;
	FUDDealData data = UUDDealActionContractExecute::ConvertData(parentAction);
	for (auto wrappedAction : targetWorldState->Deals[data.DealId]->DealActionList)
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