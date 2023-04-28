// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyInvokerAdd.h"

bool UUDDealActionPointModifyInvokerAdd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyInvokerAdd::ConvertData(data);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyInvokerAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealPointValueData data = UUDDealActionPointModifyInvokerAdd::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->Invokers.Add(action.Value);
}
void UUDDealActionPointModifyInvokerAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealPointValueData data = UUDDealActionPointModifyInvokerAdd::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->Invokers.Remove(action.Value);
}