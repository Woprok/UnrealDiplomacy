// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyInvokerRemove.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyInvokerRemove::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPointTarget data(action.ValueParameters);
	bool isInvoker = world->Deals[data.DealId]->Points[data.PointId]->Invokers.Contains(data.TargetId);
	return UUDDealActionPointModify::CanExecute(action, world) && isInvoker;
}

void UUDDealActionPointModifyInvokerRemove::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Removes target from list of invokers.
	FUDDealDataPointTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->Invokers.Remove(data.TargetId);
}

void UUDDealActionPointModifyInvokerRemove::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Add target to list of invokers.
	FUDDealDataPointTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->Invokers.Add(data.TargetId);
}