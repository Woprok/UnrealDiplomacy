// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyResetParameters.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyResetParameters::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPoint data(action.ValueParameters);
	bool hasValues = world->Deals[data.DealId]->Points[data.PointId]->ValueParameters.Num() > 0;
	bool hasText = world->Deals[data.DealId]->Points[data.PointId]->TextParameter.Len() > 0;
	return UUDDealActionPointModify::CanExecute(action, world) && (hasValues || hasText);
}

void UUDDealActionPointModifyResetParameters::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDDealDataPoint data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->ValueParameters.Empty(0);
	world->Deals[data.DealId]->Points[data.PointId]->TextParameter.Empty(0);
}

void UUDDealActionPointModifyResetParameters::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old values.
	FUDDealDataPoint data(action.ValueParameters);
	// Old data are just pasted as only original action would be able to determine what they were.
	world->Deals[data.DealId]->Points[data.PointId]->ValueParameters.Append(action.BackupValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->TextParameter = action.TextParameter;
}

void UUDDealActionPointModifyResetParameters::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDealDataPoint data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Append(world->Deals[data.DealId]->Points[data.PointId]->ValueParameters);
	action.TextParameter = world->Deals[data.DealId]->Points[data.PointId]->TextParameter;
}