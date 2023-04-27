// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionVoteNo.h"

bool UUDDealActionVoteNo::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionVoteNo::ConvertData(actionData);
		bool isReady = targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Contains(actionData.InvokerPlayerId);
		result = result && isReady;
	}
	return result;
}
void UUDDealActionVoteNo::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDDealActionVoteNo::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Remove(actionData.InvokerPlayerId);
}
void UUDDealActionVoteNo::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDDealActionVoteNo::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Add(actionData.InvokerPlayerId);
}