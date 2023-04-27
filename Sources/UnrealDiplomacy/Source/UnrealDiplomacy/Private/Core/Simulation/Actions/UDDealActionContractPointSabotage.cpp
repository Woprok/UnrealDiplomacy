// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointSabotage.h"

bool UUDDealActionContractPointSabotage::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealValueData data = UUDDealActionContractPointSabotage::ConvertData(actionData);
		bool unresolved = !targetWorldState->Deals[data.DealId]->DealActionList[data.Value].WasSabotaged;
		result = result && unresolved;
	}
	return result;
}
void UUDDealActionContractPointSabotage::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealValueData data = UUDDealActionContractPointSabotage::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].WasSabotaged = true;
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SabotageId = actionData.InvokerPlayerId;
}
void UUDDealActionContractPointSabotage::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealValueData data = UUDDealActionContractPointSabotage::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].WasSabotaged = false;
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SabotageId = UUDWorldState::GaiaWorldStateId;
}