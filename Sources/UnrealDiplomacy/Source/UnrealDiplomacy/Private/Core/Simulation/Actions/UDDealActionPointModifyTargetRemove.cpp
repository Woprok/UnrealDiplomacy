// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTargetRemove.h"

bool UUDDealActionPointModifyTargetRemove::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyTargetRemove::ConvertData(data);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyTargetRemove::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	FUDDealPointValueData data = UUDDealActionPointModifyTargetRemove::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->Targets.Remove(action.Value);
}
void UUDDealActionPointModifyTargetRemove::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	FUDDealPointValueData data = UUDDealActionPointModifyTargetRemove::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->Targets.Add(action.Value);
}
