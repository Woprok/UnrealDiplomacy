// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointTamper.h"

bool UUDDealActionContractPointTamper::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealValueData data = UUDDealActionContractPointTamper::ConvertData(data);
		bool unresolved = world->Deals[action.DealId]->DealActionList[action.Value].SelectedResult == EUDDealActionResult::Unresolved;
		result = result && unresolved;
	}
	return result;
}
void UUDDealActionContractPointTamper::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	FUDDealValueData data = UUDDealActionContractPointTamper::ConvertData(data);
	world->Deals[action.DealId]->DealActionList[action.Value].SelectedResult = EUDDealActionResult::Changed;
}
void UUDDealActionContractPointTamper::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	FUDDealValueData data = UUDDealActionContractPointTamper::ConvertData(data);
	world->Deals[action.DealId]->DealActionList[action.Value].SelectedResult = EUDDealActionResult::Unresolved;
}
