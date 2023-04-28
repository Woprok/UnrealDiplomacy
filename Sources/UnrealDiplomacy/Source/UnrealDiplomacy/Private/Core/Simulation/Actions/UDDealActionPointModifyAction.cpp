// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyAction.h"

bool UUDDealActionPointModifyAction::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(action, world);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyAction::ConvertData(action);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyAction::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDDealPointValueData newData = UUDDealActionPointModifyAction::ConvertData(action);
	world->Deals[newData.DealId]->Points[newData.Point]->ActionId = newData.Value;
}
void UUDDealActionPointModifyAction::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old value.
	FUDDealPointValueData newData = UUDDealActionPointModifyAction::ConvertData(action);
	FUDValueData oldData = UUDDealActionPointModifyAction::ConvertBackupData(action);
	world->Deals[newData.DealId]->Points[newData.Point]->ActionId = oldData.Value;
}
bool UUDDealActionPointModifyAction::IsBackupRequired()
{
	return true;
}
void UUDDealActionPointModifyAction::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDealPointValueData data = UUDDealActionPointModifyAction::ConvertData(action);
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(world->Deals[action.DealId]->Points[action.Point]->ActionId);
}

TArray<FUDActionData> UUDDealActionPointModifyAction::GetContinuations(FUDActionData& parentAction, TObjectPtr<UUDWorldState> world)
{
	FUDDealPointValueData data = UUDDealActionPointModifyAction::ConvertData(parentAction);
	return {
		FUDActionData(
			UUDCleanParametersPointDealAction::ActionTypeId,
			parentAction.InvokerPlayerId,
			{ action.DealId, action.Point }
		)
	};
}