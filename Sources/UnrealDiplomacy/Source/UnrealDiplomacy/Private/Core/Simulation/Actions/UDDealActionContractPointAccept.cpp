// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointAccept.h"

bool UUDDealActionContractPointAccept::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealValueData data = UUDDealActionContractPointAccept::ConvertData(actionData);
		bool unresolved = targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SelectedResult == EUDDealActionResult::Unresolved;
		result = result && unresolved;
	}
	return result;
}
void UUDDealActionContractPointAccept::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealValueData data = UUDDealActionContractPointAccept::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SelectedResult = EUDDealActionResult::Accepted;
}
void UUDDealActionContractPointAccept::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealValueData data = UUDDealActionContractPointAccept::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealActionList[data.Value].SelectedResult = EUDDealActionResult::Unresolved;
}