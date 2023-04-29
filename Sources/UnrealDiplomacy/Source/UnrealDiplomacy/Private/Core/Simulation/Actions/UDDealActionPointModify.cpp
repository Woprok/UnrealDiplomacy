// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModify.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModify::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	// All actions for modify have first parameter same, so we can use partial transform.
	FUDDealDataPoint data(action.ValueParameters);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isStateOpen && isResultOpen;
}