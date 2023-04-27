// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTargetRemove.h"

bool UUDDealActionPointModifyTargetRemove::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyTargetRemove::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyTargetRemove::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDDealActionPointModifyTargetRemove::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Remove(data.Value);
}
void UUDDealActionPointModifyTargetRemove::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDDealActionPointModifyTargetRemove::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Add(data.Value);
}
