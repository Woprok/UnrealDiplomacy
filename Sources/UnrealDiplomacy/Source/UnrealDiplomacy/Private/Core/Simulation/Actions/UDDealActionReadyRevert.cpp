// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionReadyRevert.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionReadyRevert::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isReady = world->Deals[data.DealId]->IsReadyPlayerList.Contains(action.InvokerFactionId);
	return IUDActionInterface::CanExecute(action, world) && isReady;
}
void UUDDealActionReadyRevert::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Unselect ready.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->IsReadyPlayerList.Remove(action.InvokerFactionId);
}
void UUDDealActionReadyRevert::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Select ready.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->IsReadyPlayerList.Add(action.InvokerFactionId);
}