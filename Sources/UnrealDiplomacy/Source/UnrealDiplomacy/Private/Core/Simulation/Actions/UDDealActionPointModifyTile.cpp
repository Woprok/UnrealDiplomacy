// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTile.h"

bool UUDDealActionPointModifyTile::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointTileData data = UUDDealActionPointModifyTile::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyTile::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointTileData newData = UUDDealActionPointModifyTile::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.X);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Y);
}
void UUDDealActionPointModifyTile::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old values.
	FUDDealPointTileData newData = UUDDealActionPointModifyTile::ConvertData(actionData);
	FUDTileData oldData = UUDDealActionPointModifyTile::ConvertBackupData(actionData);
	// Old data are replacing the new.
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.X);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Y);
}
bool UUDDealActionPointModifyTile::RequiresBackup()
{
	return true;
}
void UUDDealActionPointModifyTile::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old param is backuped for future revert use.
	FUDDealPointTileData data = UUDDealActionPointModifyTile::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	if (targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters.Num() == 0)
	{
		actionData.BackupValueParameters.Append({ 0, 0 });
	}
	else
	{
		actionData.BackupValueParameters.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters);
	}
}