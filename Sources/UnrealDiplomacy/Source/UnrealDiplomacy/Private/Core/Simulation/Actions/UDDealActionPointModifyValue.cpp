// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyValue.h"

bool UUDDealActionPointModifyValue::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyValue::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyValue::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value, we expect it to be empty due to cleanup sequence.
	FUDDealPointValueData newData = UUDDealActionPointModifyValue::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Value);
}
void UUDDealActionPointModifyValue::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old values.
	FUDDealPointValueData newData = UUDDealActionPointModifyValue::ConvertData(actionData);
	FUDValueData oldData = UUDDealActionPointModifyValue::ConvertBackupData(actionData);
	// Old data are replacing the new.
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Value);
}
bool UUDDealActionPointModifyValue::RequiresBackup()
{
	return true;
}
void UUDDealActionPointModifyValue::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old param is backuped for future revert use.
	FUDDealPointValueData data = UUDDealActionPointModifyValue::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	if (targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters.Num() == 0)
	{
		actionData.BackupValueParameters.Append({ 0 });
	}
	else
	{
		actionData.BackupValueParameters.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters);
	}
}