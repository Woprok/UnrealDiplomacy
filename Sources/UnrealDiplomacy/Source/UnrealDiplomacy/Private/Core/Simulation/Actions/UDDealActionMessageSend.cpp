// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionMessageSend.h"

bool UUDDealActionMessageSend::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionMessageSend::ConvertData(actionData);
		bool isNotEmpty = actionData.TextParameter.Len() > 0;
		result = result && isNotEmpty;
	}
	return result;
}
void UUDDealActionMessageSend::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Add to history.
	FUDDealData data = UUDDealActionMessageSend::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->ChatHistory.Add(actionData.TextParameter);
}
void UUDDealActionMessageSend::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove last add message from the history.
	FUDDealData data = UUDDealActionMessageSend::ConvertData(actionData);
	int32 lastItem = targetWorldState->Deals[data.DealId]->ChatHistory.Num() - 1;
	targetWorldState->Deals[data.DealId]->ChatHistory.RemoveAt(lastItem);
}