// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyInvokerRemove.h"

bool UUDDealActionPointModifyInvokerRemove::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyInvokerRemove::ConvertData(data);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyInvokerRemove::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	FUDDealPointValueData data = UUDDealActionPointModifyInvokerRemove::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->Invokers.Remove(action.Value);
}
void UUDDealActionPointModifyInvokerRemove::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	FUDDealPointValueData data = UUDDealActionPointModifyInvokerRemove::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->Invokers.Add(action.Value);
}