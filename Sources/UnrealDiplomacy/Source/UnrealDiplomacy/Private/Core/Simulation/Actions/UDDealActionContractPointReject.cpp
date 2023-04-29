// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointReject.h"

bool UUDDealActionContractPointReject::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealValueData data = UUDDealActionContractPointReject::ConvertData(data);
		bool unresolved = world->Deals[action.DealId]->DealActionList[action.Value].SelectedResult == EUDDealActionResult::Unresolved;
		result = result && unresolved;
	}
	return result;
}
void UUDDealActionContractPointReject::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	FUDDealValueData data = UUDDealActionContractPointReject::ConvertData(data);
	world->Deals[action.DealId]->DealActionList[action.Value].SelectedResult = EUDDealActionResult::Denied;
}
void UUDDealActionContractPointReject::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	FUDDealValueData data = UUDDealActionContractPointReject::ConvertData(data);
	world->Deals[action.DealId]->DealActionList[action.Value].SelectedResult = EUDDealActionResult::Unresolved;
}
