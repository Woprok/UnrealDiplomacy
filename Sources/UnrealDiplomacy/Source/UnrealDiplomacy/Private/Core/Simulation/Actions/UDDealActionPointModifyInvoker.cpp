// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyInvoker.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyInvoker::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPointTarget data(action.ValueParameters);
	bool isNotInvoker = world->Deals[data.DealId]->Points[data.PointId]->Invoker != data.TargetId;
	return UUDDealActionPointModify::CanExecute(action, world) && isNotInvoker;
}

void UUDDealActionPointModifyInvoker::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Add target as invoker.
	FUDDealDataPointTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->Invoker = data.TargetId;
}

void UUDDealActionPointModifyInvoker::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to backup value
	FUDDealDataPointTarget data(action.ValueParameters);
	FUDDealDataPointTarget oldData(action.BackupValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->Invoker = oldData.TargetId;
}

void UUDDealActionPointModifyInvoker::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDealDataPointTarget data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(data.DealId);
	action.BackupValueParameters.Add(data.PointId);
	action.BackupValueParameters.Add(world->Deals[data.DealId]->Points[data.PointId]->Invoker);
}