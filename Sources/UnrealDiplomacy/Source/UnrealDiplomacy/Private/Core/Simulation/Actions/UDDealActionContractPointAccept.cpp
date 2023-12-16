// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointAccept.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionContractPointAccept::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataContractPoint data(action.ValueParameters);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::ResolutionOfPoints;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	bool isNotResolved = world->Deals[data.DealId]->DealActionList[data.ContractPointId].SelectedResult == EUDDealActionResult::Unresolved;
	return IUDActionInterface::CanExecute(action, world) && isNotResolved && isStateOpen && isResultOpen;
}

void UUDDealActionContractPointAccept::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Accept contract point.
	FUDDealDataContractPoint data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SelectedResult = EUDDealActionResult::Accepted;
}

void UUDDealActionContractPointAccept::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revert to undecided state of contract point.
	FUDDealDataContractPoint data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SelectedResult = EUDDealActionResult::Unresolved;
}