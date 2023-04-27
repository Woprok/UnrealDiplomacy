// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyType.h"

bool UUDDealActionPointModifyType::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyType::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyType::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointValueData newData = UUDDealActionPointModifyType::ConvertData(actionData);
	EUDPointType pointType = UUDDealActionPointModifyType::IntegerToPointType(newData.Value);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->Type = pointType;
}
void UUDDealActionPointModifyType::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old value.
	FUDDealPointValueData newData = UUDDealActionPointModifyType::ConvertData(actionData);
	FUDValueData oldData = UUDDealActionPointModifyType::ConvertBackupData(actionData);
	EUDPointType pointType = UUDDealActionPointModifyType::IntegerToPointType(oldData.Value);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->Type = pointType;
}
bool UUDDealActionPointModifyType::RequiresBackup()
{
	return true;
}
void UUDDealActionPointModifyType::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old action is backuped for future revert use.
	FUDDealPointValueData data = UUDDealActionPointModifyType::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	int32 pointType = UUDDealActionPointModifyType::PointTypeToInteger(targetWorldState->Deals[data.DealId]->Points[data.Point]->Type);
	actionData.BackupValueParameters.Add(pointType);
}