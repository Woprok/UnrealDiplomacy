// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointTamper.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionContractPointTamper::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataContractPoint data(action.ValueParameters);
	bool isNotResolved = world->Deals[data.DealId]->DealActionList[data.ContractPointId].SelectedResult == EUDDealActionResult::Unresolved;
	return IUDActionInterface::CanExecute(action, world) && isNotResolved;
}

void UUDDealActionContractPointTamper::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Tampers with the contract point.
	FUDDealDataContractPoint data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SelectedResult = EUDDealActionResult::Changed;
}

void UUDDealActionContractPointTamper::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revert to undecided state of contract point.
	FUDDealDataContractPoint data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SelectedResult = EUDDealActionResult::Unresolved;
}