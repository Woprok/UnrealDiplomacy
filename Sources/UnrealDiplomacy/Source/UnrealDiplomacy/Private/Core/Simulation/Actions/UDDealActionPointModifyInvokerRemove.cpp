// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyInvokerRemove.h"

bool UUDDealActionPointModifyInvokerRemove::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyInvokerRemove::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyInvokerRemove::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDDealActionPointModifyInvokerRemove::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Remove(data.Value);
}
void UUDDealActionPointModifyInvokerRemove::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDDealActionPointModifyInvokerRemove::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Add(data.Value);
}