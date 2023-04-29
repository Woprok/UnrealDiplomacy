// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyType.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyType::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPointValue data(action.ValueParameters);

	return UUDDealActionPointModify::CanExecute(action, world);
}

void UUDDealActionPointModifyType::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDDealDataPointValue newData(action.ValueParameters);
	EUDPointType pointType = UUDDealActionPointModifyType::IntegerToPointType(newData.Value);
	world->Deals[newData.DealId]->Points[newData.PointId]->Type = pointType;
}

void UUDDealActionPointModifyType::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old value.
	FUDDealDataPointValue newData(action.ValueParameters);
	FUDDealDataPointValue oldData(action.BackupValueParameters);
	EUDPointType pointType = UUDDealActionPointModifyType::IntegerToPointType(oldData.Value);
	world->Deals[newData.DealId]->Points[newData.PointId]->Type = pointType;
}

void UUDDealActionPointModifyType::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDealDataPointValue data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	int32 pointType = UUDDealActionPointModifyType::PointTypeToInteger(world->Deals[data.DealId]->Points[data.PointId]->Type);
	action.BackupValueParameters.Add(pointType);
}