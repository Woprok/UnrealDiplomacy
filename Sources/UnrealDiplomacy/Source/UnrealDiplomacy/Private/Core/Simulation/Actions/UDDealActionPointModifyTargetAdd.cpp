// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTargetAdd.h"

bool UUDDealActionPointModifyTargetAdd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyTargetAdd::ConvertData(data);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyTargetAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealPointValueData data = UUDDealActionPointModifyTargetAdd::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->Targets.Add(action.Value);
}
void UUDDealActionPointModifyTargetAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealPointValueData data = UUDDealActionPointModifyTargetAdd::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->Targets.Remove(action.Value);
}