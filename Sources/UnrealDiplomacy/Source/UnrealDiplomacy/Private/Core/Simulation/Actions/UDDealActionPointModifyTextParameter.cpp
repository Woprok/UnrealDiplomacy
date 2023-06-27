// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTextParameter.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyTextParameter::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPoint data(action.ValueParameters);
	bool isNotSame = world->Deals[data.DealId]->Points[data.PointId]->TextParameter != action.TextParameter;
	return UUDDealActionPointModify::CanExecute(action, world) && isNotSame;
}

void UUDDealActionPointModifyTextParameter::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Add target to list of targets.
	FUDDealDataPoint data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->TextParameter = action.TextParameter;
}

void UUDDealActionPointModifyTextParameter::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Removes target from list of targets.
	FUDDealDataPoint data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->TextParameter = action.BackupTextParameter;
}

void UUDDealActionPointModifyTextParameter::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old faction name is backuped for future revert use.
	FUDDealDataPoint data(action.ValueParameters);
	action.BackupTextParameter = world->Deals[data.DealId]->Points[data.PointId]->TextParameter;
}