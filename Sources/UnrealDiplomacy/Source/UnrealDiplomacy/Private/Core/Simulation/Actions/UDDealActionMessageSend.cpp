// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionMessageSend.h"

bool UUDDealActionMessageSend::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionMessageSend::ConvertData(data);
		bool isNotEmpty = action.TextParameter.Len() > 0;
		result = result && isNotEmpty;
	}
	return result;
}
void UUDDealActionMessageSend::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Add to history.
	FUDDealData data = UUDDealActionMessageSend::ConvertData(data);
	world->Deals[action.DealId]->ChatHistory.Add(action.TextParameter);
}
void UUDDealActionMessageSend::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Remove last add message from the history.
	FUDDealData data = UUDDealActionMessageSend::ConvertData(data);
	int32 lastItem = world->Deals[action.DealId]->ChatHistory.Num() - 1;
	world->Deals[action.DealId]->ChatHistory.RemoveAt(lastItem);
}