// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyResetParameters.h"

bool UUDDealActionPointModifyResetParameters::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointData data = UUDDealActionPointModifyResetParameters::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyResetParameters::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointData newData = UUDDealActionPointModifyResetParameters::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->TextParameter.Empty(0);
}
void UUDDealActionPointModifyResetParameters::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old values.
	FUDDealPointData newData = UUDDealActionPointModifyResetParameters::ConvertData(actionData);
	// Old data are just pasted as only original action would be able to determine what they were.
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Append(actionData.BackupValueParameters);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->TextParameter.Append(actionData.TextParameter);
}
bool UUDDealActionPointModifyResetParameters::RequiresBackup()
{
	return true;
}
void UUDDealActionPointModifyResetParameters::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old action is backuped for future revert use.
	FUDDealPointData data = UUDDealActionPointModifyResetParameters::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	actionData.BackupValueParameters.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters);
	actionData.TextParameter.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->TextParameter);
}