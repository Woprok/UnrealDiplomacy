// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionReady.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionReady::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isNotReady = !world->Deals[data.DealId]->IsReadyPlayerList.Contains(action.InvokerPlayerId);
	return IUDActionInterface::CanExecute(action, world) && isNotReady;
}
void UUDDealActionReady::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Select ready.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->IsReadyPlayerList.Add(action.InvokerPlayerId);
}
void UUDDealActionReady::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Unselect ready.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->IsReadyPlayerList.Remove(action.InvokerPlayerId);
}