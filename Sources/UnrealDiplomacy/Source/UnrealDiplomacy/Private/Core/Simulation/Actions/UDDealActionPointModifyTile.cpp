// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTile.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyTile::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPointTile data(action.ValueParameters);

	return UUDDealActionPointModify::CanExecute(action, world);
}

void UUDDealActionPointModifyTile::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDDealDataPointTile newData(action.ValueParameters);
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Add(newData.X);
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Add(newData.Y);
}

void UUDDealActionPointModifyTile::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old values.
	FUDDealDataPointTile newData(action.ValueParameters);
	FUDDealDataPointTile oldData(action.BackupValueParameters);
	// Old data are replacing the new.
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Add(oldData.X);
	world->Deals[newData.DealId]->Points[newData.PointId]->ValueParameters.Add(oldData.Y);
}

void UUDDealActionPointModifyTile::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old param is backuped for future revert use.
	FUDDealDataPointTile data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(data.DealId);
	action.BackupValueParameters.Add(data.PointId);
	if (world->Deals[data.DealId]->Points[data.PointId]->ValueParameters.Num() == 0)
	{
		action.BackupValueParameters.Append({ 0, 0 });
	}
	else
	{
		action.BackupValueParameters.Append(world->Deals[data.DealId]->Points[data.PointId]->ValueParameters);
	}
}