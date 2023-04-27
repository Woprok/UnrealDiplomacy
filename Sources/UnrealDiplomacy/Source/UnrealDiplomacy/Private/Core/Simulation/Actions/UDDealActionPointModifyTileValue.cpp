// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTileValue.h"

bool UUDDealActionPointModifyTileValue::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointTileValueData data = UUDDealActionPointModifyTileValue::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyTileValue::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointTileValueData newData = UUDDealActionPointModifyTileValue::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.X);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Y);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Value);
}
void UUDDealActionPointModifyTileValue::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old values.
	FUDDealPointTileValueData newData = UUDDealActionPointModifyTileValue::ConvertData(actionData);
	FUDTileValueData oldData = UUDDealActionPointModifyTileValue::ConvertBackupData(actionData);
	// Old data are replacing the new.
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.X);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Y);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Value);
}
bool UUDDealActionPointModifyTileValue::RequiresBackup()
{
	return true;
}
void UUDDealActionPointModifyTileValue::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old param is backuped for future revert use.
	FUDDealPointTileValueData data = UUDDealActionPointModifyTileValue::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	if (targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters.Num() == 0)
	{
		actionData.BackupValueParameters.Append({ 0, 0, 0 });
	}
	else
	{
		actionData.BackupValueParameters.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters);
	}
}