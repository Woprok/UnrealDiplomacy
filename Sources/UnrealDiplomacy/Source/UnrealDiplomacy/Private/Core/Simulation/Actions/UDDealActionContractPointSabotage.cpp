// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointSabotage.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionContractPointSabotage::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataContractPoint data(action.ValueParameters);
	bool isNotSabotaged = !world->Deals[data.DealId]->DealActionList[data.ContractPointId].WasSabotaged;
	return IUDActionInterface::CanExecute(action, world) && isNotSabotaged;
}

void UUDDealActionContractPointSabotage::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Apply sabotage flag.
	FUDDealDataContractPoint data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].WasSabotaged = true;
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SabotageId = action.InvokerPlayerId;
}

void UUDDealActionContractPointSabotage::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Removes sabotage flag.
	FUDDealDataContractPoint data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].WasSabotaged = false;
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SabotageId = UUDGlobalData::GaiaId;
}