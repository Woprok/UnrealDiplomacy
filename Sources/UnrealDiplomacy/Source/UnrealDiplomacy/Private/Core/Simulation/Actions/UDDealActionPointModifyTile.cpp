// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyTile.h"

bool UUDDealActionPointModifyTile::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(action, world);
	if (result)
	{
		FUDDealPointTileData data = UUDDealActionPointModifyTile::ConvertData(action);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyTile::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDDealPointTileData newData = UUDDealActionPointModifyTile::ConvertData(action);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.X);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Y);
}
void UUDDealActionPointModifyTile::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old values.
	FUDDealPointTileData newData = UUDDealActionPointModifyTile::ConvertData(action);
	FUDTileData oldData = UUDDealActionPointModifyTile::ConvertBackupData(action);
	// Old data are replacing the new.
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.X);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Y);
}
bool UUDDealActionPointModifyTile::IsBackupRequired()
{
	return true;
}
void UUDDealActionPointModifyTile::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old param is backuped for future revert use.
	FUDDealPointTileData data = UUDDealActionPointModifyTile::ConvertData(action);
	action.BackupValueParameters.Empty(0);
	if (world->Deals[action.DealId]->Points[action.Point]->ValueParameters.Num() == 0)
	{
		action.BackupValueParameters.Append({ 0, 0 });
	}
	else
	{
		action.BackupValueParameters.Append(world->Deals[action.DealId]->Points[action.Point]->ValueParameters);
	}
}