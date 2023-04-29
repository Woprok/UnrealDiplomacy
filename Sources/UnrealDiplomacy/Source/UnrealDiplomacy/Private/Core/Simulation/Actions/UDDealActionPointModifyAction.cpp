// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyAction.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyResetParameters.h"

bool UUDDealActionPointModifyAction::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPointValue data(action.ValueParameters);

	return UUDDealActionPointModify::CanExecute(action, world);
}

void UUDDealActionPointModifyAction::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDDealDataPointValue newData(action.ValueParameters);
	world->Deals[newData.DealId]->Points[newData.PointId]->ActionId = newData.Value;
}

void UUDDealActionPointModifyAction::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old value.
	FUDDealDataPointValue newData(action.ValueParameters);
	FUDDealDataPointValue oldData(action.BackupValueParameters);
	world->Deals[newData.DealId]->Points[newData.PointId]->ActionId = oldData.Value;
}

void UUDDealActionPointModifyAction::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDealDataPointValue data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(data.DealId);
	action.BackupValueParameters.Add(data.PointId);
	action.BackupValueParameters.Add(world->Deals[data.DealId]->Points[data.PointId]->ActionId);
}

TArray<FUDActionData> UUDDealActionPointModifyAction::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world)
{
	FUDDealDataPointValue data(parentAction.ValueParameters);
	return {
		FUDActionData(
			UUDDealActionPointModifyResetParameters::ActionTypeId,
			parentAction.InvokerPlayerId,
			{ data.DealId, data.PointId }
		)
	};
}