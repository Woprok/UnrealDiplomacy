// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyValue.h"

bool UUDDealActionPointModifyValue::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(action, world);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyValue::ConvertData(action);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyValue::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value, we expect it to be empty due to cleanup sequence.
	FUDDealPointValueData newData = UUDDealActionPointModifyValue::ConvertData(action);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Value);
}
void UUDDealActionPointModifyValue::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old values.
	FUDDealPointValueData newData = UUDDealActionPointModifyValue::ConvertData(action);
	FUDValueData oldData = UUDDealActionPointModifyValue::ConvertBackupData(action);
	// Old data are replacing the new.
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Value);
}
bool UUDDealActionPointModifyValue::IsBackupRequired()
{
	return true;
}
void UUDDealActionPointModifyValue::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old param is backuped for future revert use.
	FUDDealPointValueData data = UUDDealActionPointModifyValue::ConvertData(action);
	action.BackupValueParameters.Empty(0);
	if (world->Deals[action.DealId]->Points[action.Point]->ValueParameters.Num() == 0)
	{
		action.BackupValueParameters.Append({ 0 });
	}
	else
	{
		action.BackupValueParameters.Append(world->Deals[action.DealId]->Points[action.Point]->ValueParameters);
	}
}