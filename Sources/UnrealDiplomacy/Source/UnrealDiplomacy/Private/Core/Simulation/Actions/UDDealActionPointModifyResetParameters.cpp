// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyResetParameters.h"

bool UUDDealActionPointModifyResetParameters::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(action, world);
	if (result)
	{
		FUDDealPointData data = UUDDealActionPointModifyResetParameters::ConvertData(action);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyResetParameters::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDDealPointData newData = UUDDealActionPointModifyResetParameters::ConvertData(action);
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	world->Deals[newData.DealId]->Points[newData.Point]->TextParameter.Empty(0);
}
void UUDDealActionPointModifyResetParameters::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old values.
	FUDDealPointData newData = UUDDealActionPointModifyResetParameters::ConvertData(action);
	// Old data are just pasted as only original action would be able to determine what they were.
	world->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Append(action.BackupValueParameters);
	world->Deals[newData.DealId]->Points[newData.Point]->TextParameter.Append(action.TextParameter);
}
bool UUDDealActionPointModifyResetParameters::IsBackupRequired()
{
	return true;
}
void UUDDealActionPointModifyResetParameters::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDealPointData data = UUDDealActionPointModifyResetParameters::ConvertData(action);
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Append(world->Deals[action.DealId]->Points[action.Point]->ValueParameters);
	action.TextParameter.Append(world->Deals[action.DealId]->Points[action.Point]->TextParameter);
}