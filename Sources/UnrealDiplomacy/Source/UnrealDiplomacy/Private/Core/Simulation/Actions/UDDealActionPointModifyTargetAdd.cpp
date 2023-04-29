// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTargetAdd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyTargetAdd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPointTarget data(action.ValueParameters);
	bool isNotTarget = !world->Deals[data.DealId]->Points[data.PointId]->Targets.Contains(data.TargetId);
	return UUDDealActionPointModify::CanExecute(action, world) && isNotTarget;
}

void UUDDealActionPointModifyTargetAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Add target to list of targets.
	FUDDealDataPointTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->Targets.Add(data.TargetId);
}

void UUDDealActionPointModifyTargetAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Removes target from list of targets.
	FUDDealDataPointTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->Targets.Remove(data.TargetId);
}