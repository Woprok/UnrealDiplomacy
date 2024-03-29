// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointSabotage.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionContractPointSabotage::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataContractPoint data(action.ValueParameters);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::ResolutionOfPoints;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	bool isNotSabotaged = !world->Deals[data.DealId]->DealActionList[data.ContractPointId].WasSabotaged;
	return IUDActionInterface::CanExecute(action, world) && isNotSabotaged && isStateOpen && isResultOpen;
}

void UUDDealActionContractPointSabotage::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Apply sabotage flag.
	FUDDealDataContractPoint data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].WasSabotaged = true;
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SabotageId = action.InvokerFactionId;
}

void UUDDealActionContractPointSabotage::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Removes sabotage flag.
	FUDDealDataContractPoint data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].WasSabotaged = false;
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SabotageId = UUDGlobalData::GaiaFactionId;
}