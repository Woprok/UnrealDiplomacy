// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointAccept.h"

bool UUDDealActionContractPointAccept::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealValueData data = UUDDealActionContractPointAccept::ConvertData(data);
		bool unresolved = world->Deals[action.DealId]->DealActionList[action.Value].SelectedResult == EUDDealActionResult::Unresolved;
		result = result && unresolved;
	}
	return result;
}
void UUDDealActionContractPointAccept::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	FUDDealValueData data = UUDDealActionContractPointAccept::ConvertData(data);
	world->Deals[action.DealId]->DealActionList[action.Value].SelectedResult = EUDDealActionResult::Accepted;
}
void UUDDealActionContractPointAccept::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	FUDDealValueData data = UUDDealActionContractPointAccept::ConvertData(data);
	world->Deals[action.DealId]->DealActionList[action.Value].SelectedResult = EUDDealActionResult::Unresolved;
}