// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionReady.h"

bool UUDDealActionReady::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionReady::ConvertData(data);
		bool isNotReady = !world->Deals[action.DealId]->IsReadyPlayerList.Contains(action.InvokerPlayerId);
		result = result && isNotReady;
	}
	return result;
}
void UUDDealActionReady::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealData data = UUDDealActionReady::ConvertData(data);
	world->Deals[action.DealId]->IsReadyPlayerList.Add(action.InvokerPlayerId);
}
void UUDDealActionReady::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealData data = UUDDealActionReady::ConvertData(data);
	world->Deals[action.DealId]->IsReadyPlayerList.Remove(action.InvokerPlayerId);
}