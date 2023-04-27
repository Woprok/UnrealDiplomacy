// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionReady.h"

bool UUDDealActionReady::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionReady::ConvertData(actionData);
		bool isNotReady = !targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Contains(actionData.InvokerPlayerId);
		result = result && isNotReady;
	}
	return result;
}
void UUDDealActionReady::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDDealActionReady::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Add(actionData.InvokerPlayerId);
}
void UUDDealActionReady::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDDealActionReady::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Remove(actionData.InvokerPlayerId);
}