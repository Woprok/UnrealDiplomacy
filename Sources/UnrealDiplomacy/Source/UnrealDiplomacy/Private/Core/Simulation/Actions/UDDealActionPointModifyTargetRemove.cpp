// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTargetRemove.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyTargetRemove::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPointTarget data(action.ValueParameters);
	bool isTarget = world->Deals[data.DealId]->Points[data.PointId]->Targets.Contains(data.TargetId);
	return UUDDealActionPointModify::CanExecute(action, world) && isTarget;
}

void UUDDealActionPointModifyTargetRemove::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Removes target from list of targets.
	FUDDealDataPointTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->Targets.Remove(data.TargetId);
}

void UUDDealActionPointModifyTargetRemove::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Add target to list of targets.
	FUDDealDataPointTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->Targets.Add(data.TargetId);
}