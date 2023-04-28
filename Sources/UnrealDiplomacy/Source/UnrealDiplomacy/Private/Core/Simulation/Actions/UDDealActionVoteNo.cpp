// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionVoteNo.h"

bool UUDDealActionVoteNo::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionVoteNo::ConvertData(data);
		bool isReady = world->Deals[action.DealId]->PositiveVotePlayerList.Contains(action.InvokerPlayerId);
		result = result && isReady;
	}
	return result;
}
void UUDDealActionVoteNo::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealData data = UUDDealActionVoteNo::ConvertData(data);
	world->Deals[action.DealId]->PositiveVotePlayerList.Remove(action.InvokerPlayerId);
}
void UUDDealActionVoteNo::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealData data = UUDDealActionVoteNo::ConvertData(data);
	world->Deals[action.DealId]->PositiveVotePlayerList.Add(action.InvokerPlayerId);
}