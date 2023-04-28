// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionReadyRevert.h"


bool UUDDealActionReadyRevert::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionReadyRevert::ConvertData(data);
		bool isReady = world->Deals[action.DealId]->IsReadyPlayerList.Contains(action.InvokerPlayerId);
		result = result && isReady;
	}
	return result;
}
void UUDDealActionReadyRevert::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealData data = UUDDealActionReadyRevert::ConvertData(data);
	world->Deals[action.DealId]->IsReadyPlayerList.Remove(action.InvokerPlayerId);
}
void UUDDealActionReadyRevert::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealData data = UUDDealActionReadyRevert::ConvertData(data);
	world->Deals[action.DealId]->IsReadyPlayerList.Add(action.InvokerPlayerId);
}
