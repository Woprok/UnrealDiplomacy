// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionVoteYes.h"

bool UUDDealActionVoteYes::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionVoteYes::ConvertData(actionData);
		bool isNotReady = !targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Contains(actionData.InvokerPlayerId);
		result = result && isNotReady;
	}
	return result;
}
void UUDDealActionVoteYes::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDDealActionVoteYes::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Add(actionData.InvokerPlayerId);
}
void UUDDealActionVoteYes::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDDealActionVoteYes::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Remove(actionData.InvokerPlayerId);
}