// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointIgnore.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointIgnore::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPoint data(action.ValueParameters);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
	bool isNotIgnored = world->Deals[data.DealId]->Points[data.PointId]->IsIgnored == false;
	return IUDActionInterface::CanExecute(action, world) && isStateOpen && isResultOpen;
}

void UUDDealActionPointIgnore::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Mark point as ignored.
	FUDDealDataPoint data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->IsIgnored = true;
}

void UUDDealActionPointIgnore::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove tag from the point.
	FUDDealDataPoint data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->IsIgnored = false;
}