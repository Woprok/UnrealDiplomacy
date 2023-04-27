// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionReadyRevert.h"


bool UUDDealActionReadyRevert::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionReadyRevert::ConvertData(actionData);
		bool isReady = targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Contains(actionData.InvokerPlayerId);
		result = result && isReady;
	}
	return result;
}
void UUDDealActionReadyRevert::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDDealActionReadyRevert::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Remove(actionData.InvokerPlayerId);
}
void UUDDealActionReadyRevert::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDDealActionReadyRevert::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Add(actionData.InvokerPlayerId);
}
