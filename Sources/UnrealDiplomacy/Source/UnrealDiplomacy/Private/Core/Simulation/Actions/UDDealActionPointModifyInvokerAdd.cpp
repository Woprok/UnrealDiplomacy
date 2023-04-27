// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyInvokerAdd.h"

bool UUDDealActionPointModifyInvokerAdd::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyInvokerAdd::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyInvokerAdd::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDDealActionPointModifyInvokerAdd::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Add(data.Value);
}
void UUDDealActionPointModifyInvokerAdd::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDDealActionPointModifyInvokerAdd::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Remove(data.Value);
}