// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointReject.h"

bool UUDDealActionContractPointReject::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealValueData data = UUDDealActionContractPointReject::ConvertData(actionData);
		bool unresolved = targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SelectedResult == EUDDealActionResult::Unresolved;
		result = result && unresolved;
	}
	return result;
}
void UUDDealActionContractPointReject::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealValueData data = UUDDealActionContractPointReject::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SelectedResult = EUDDealActionResult::Denied;
}
void UUDDealActionContractPointReject::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealValueData data = UUDDealActionContractPointReject::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SelectedResult = EUDDealActionResult::Unresolved;
}
