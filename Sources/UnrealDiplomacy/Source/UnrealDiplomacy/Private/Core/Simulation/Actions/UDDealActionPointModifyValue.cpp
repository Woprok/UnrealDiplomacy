// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyValue.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyValue::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPointValue data(action.ValueParameters);

	return UUDDealActionPointModify::CanExecute(action, world);
}

void UUDDealActionPointModifyValue::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value, we expect it to be empty due to cleanup sequence.
	FUDDealDataPointValue newData(action.ValueParameters);
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Add(newData.Value);
}

void UUDDealActionPointModifyValue::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old values.
	FUDDealDataPointValue newData(action.ValueParameters);
	FUDDealDataPointValue oldData(action.BackupValueParameters);
	// Old data are replacing the new.
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Add(oldData.Value);
}

void UUDDealActionPointModifyValue::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old param is backuped for future revert use.
	FUDDealDataPointValue data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(data.DealId);
	action.BackupValueParameters.Add(data.PointId);
	if (world->Deals[data.DealId]->Points[data.PointId]->ValueParameters.Num() == 0)
	{
		action.BackupValueParameters.Append({ 0 });
	}
	else
	{
		action.BackupValueParameters.Append(world->Deals[data.DealId]->Points[data.PointId]->ValueParameters);
	}
}