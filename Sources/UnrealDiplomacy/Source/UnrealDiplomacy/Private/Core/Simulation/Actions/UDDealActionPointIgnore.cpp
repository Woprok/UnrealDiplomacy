// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointIgnore.h"

bool UUDDealActionPointIgnore::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealPointData data = UUDDealActionPointIgnore::ConvertData(data);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isNotIgnored = world->Deals[action.DealId]->Points[action.Point]->IsIgnored == false;
		result = result && isStateOpen && isResultOpen && isNotIgnored;
	}
	return result;
}
void UUDDealActionPointIgnore::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Mark point as ignored. Removing is not necessary.
	// TODO consider Id based indexing for Points...
	FUDDealPointData data = UUDDealActionPointIgnore::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->IsIgnored = true;
}
void UUDDealActionPointIgnore::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Mark point as available for editing and use.
	FUDDealPointData data = UUDDealActionPointIgnore::ConvertData(data);
	world->Deals[action.DealId]->Points[action.Point]->IsIgnored = false;
}
