// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTileValue.h"

bool UUDDealActionPointModifyTileValue::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(action, world);
	if (result)
	{
		FUDDealPointTileValueData data = UUDDealActionPointModifyTileValue::ConvertData(action);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyTileValue::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDDealPointTileValueData newData = UUDDealActionPointModifyTileValue::ConvertData(action);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.X);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Y);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Value);
}
void UUDDealActionPointModifyTileValue::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old values.
	FUDDealPointTileValueData newData = UUDDealActionPointModifyTileValue::ConvertData(action);
	FUDTileValueData oldData = UUDDealActionPointModifyTileValue::ConvertBackupData(action);
	// Old data are replacing the new.
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.X);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Y);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Value);
}
bool UUDDealActionPointModifyTileValue::IsBackupRequired()
{
	return true;
}
void UUDDealActionPointModifyTileValue::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old param is backuped for future revert use.
	FUDDealPointTileValueData data = UUDDealActionPointModifyTileValue::ConvertData(action);
	action.BackupValueParameters.Empty(0);
	if (world->Deals[action.DealId]->Points[action.Point]->ValueParameters.Num() == 0)
	{
		action.BackupValueParameters.Append({ 0, 0, 0 });
	}
	else
	{
		action.BackupValueParameters.Append(world->Deals[action.DealId]->Points[action.Point]->ValueParameters);
	}
}