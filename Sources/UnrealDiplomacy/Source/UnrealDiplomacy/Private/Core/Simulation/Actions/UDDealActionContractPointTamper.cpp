// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointTamper.h"

bool UUDDealActionContractPointTamper::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealValueData data = UUDDealActionContractPointTamper::ConvertData(actionData);
		bool unresolved = targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SelectedResult == EUDDealActionResult::Unresolved;
		result = result && unresolved;
	}
	return result;
}
void UUDDealActionContractPointTamper::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealValueData data = UUDDealActionContractPointTamper::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SelectedResult = EUDDealActionResult::Changed;
}
void UUDDealActionContractPointTamper::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealValueData data = UUDDealActionContractPointTamper::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SelectedResult = EUDDealActionResult::Unresolved;
}
