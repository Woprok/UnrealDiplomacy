// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointSabotage.h"

bool UUDDealActionContractPointSabotage::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealValueData data = UUDDealActionContractPointSabotage::ConvertData(data);
		bool unresolved = !world->Deals[action.DealId]->DealActionList[action.Value].WasSabotaged;
		result = result && unresolved;
	}
	return result;
}
void UUDDealActionContractPointSabotage::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	FUDDealValueData data = UUDDealActionContractPointSabotage::ConvertData(data);
	world->Deals[action.DealId]->DealActionList[action.Value].WasSabotaged = true;
	world->Deals[action.DealId]->DealActionList[action.Value].SabotageId = action.InvokerPlayerId;
}
void UUDDealActionContractPointSabotage::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	FUDDealValueData data = UUDDealActionContractPointSabotage::ConvertData(data);
	world->Deals[action.DealId]->DealActionList[action.Value].WasSabotaged = false;
	world->Deals[action.DealId]->DealActionList[action.Value].SabotageId = UUDWorldState::GaiaWorldStateId;
}