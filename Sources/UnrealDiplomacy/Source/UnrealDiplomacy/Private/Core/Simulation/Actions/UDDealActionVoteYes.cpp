// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionVoteYes.h"

bool UUDDealActionVoteYes::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionVoteYes::ConvertData(data);
		bool isNotReady = !world->Deals[action.DealId]->PositiveVotePlayerList.Contains(action.InvokerPlayerId);
		result = result && isNotReady;
	}
	return result;
}
void UUDDealActionVoteYes::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealData data = UUDDealActionVoteYes::ConvertData(data);
	world->Deals[action.DealId]->PositiveVotePlayerList.Add(action.InvokerPlayerId);
}
void UUDDealActionVoteYes::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealData data = UUDDealActionVoteYes::ConvertData(data);
	world->Deals[action.DealId]->PositiveVotePlayerList.Remove(action.InvokerPlayerId);
}