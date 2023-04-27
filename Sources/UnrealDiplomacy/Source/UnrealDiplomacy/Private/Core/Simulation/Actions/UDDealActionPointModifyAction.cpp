// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyAction.h"

bool UUDDealActionPointModifyAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointValueData newData = UUDDealActionPointModifyAction::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ActionId = newData.Value;
}
void UUDDealActionPointModifyAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old value.
	FUDDealPointValueData newData = UUDDealActionPointModifyAction::ConvertData(actionData);
	FUDValueData oldData = UUDDealActionPointModifyAction::ConvertBackupData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ActionId = oldData.Value;
}
bool UUDDealActionPointModifyAction::RequiresBackup()
{
	return true;
}
void UUDDealActionPointModifyAction::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old action is backuped for future revert use.
	FUDDealPointValueData data = UUDDealActionPointModifyAction::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	actionData.BackupValueParameters.Add(targetWorldState->Deals[data.DealId]->Points[data.Point]->ActionId);
}

TArray<FUDActionData> UUDDealActionPointModifyAction::GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealPointValueData data = UUDDealActionPointModifyAction::ConvertData(parentAction);
	return {
		FUDActionData(
			UUDCleanParametersPointDealAction::ActionTypeId,
			parentAction.InvokerPlayerId,
			{ data.DealId, data.Point }
		)
	};
}