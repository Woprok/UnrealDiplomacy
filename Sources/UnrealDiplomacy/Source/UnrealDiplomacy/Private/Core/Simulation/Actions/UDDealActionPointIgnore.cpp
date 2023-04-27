// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointIgnore.h"

bool UUDDealActionPointIgnore::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointData data = UUDDealActionPointIgnore::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isNotIgnored = targetWorldState->Deals[data.DealId]->Points[data.Point]->IsIgnored == false;
		result = result && isStateOpen && isResultOpen && isNotIgnored;
	}
	return result;
}
void UUDDealActionPointIgnore::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Mark point as ignored. Removing is not necessary.
	// TODO consider Id based indexing for Points...
	FUDDealPointData data = UUDDealActionPointIgnore::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->IsIgnored = true;
}
void UUDDealActionPointIgnore::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Mark point as available for editing and use.
	FUDDealPointData data = UUDDealActionPointIgnore::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->IsIgnored = false;
}
