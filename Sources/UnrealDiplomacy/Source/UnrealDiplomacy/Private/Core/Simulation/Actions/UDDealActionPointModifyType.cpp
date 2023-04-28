// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyType.h"

bool UUDDealActionPointModifyType::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(action, world);
	if (result)
	{
		FUDDealPointValueData data = UUDDealActionPointModifyType::ConvertData(action);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointModifyType::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDDealPointValueData newData = UUDDealActionPointModifyType::ConvertData(action);
	EUDPointType pointType = UUDDealActionPointModifyType::IntegerToPointType(newData.Value);
	world->Deals[newData.DealId]->Points[newData.Point]->Type = pointType;
}
void UUDDealActionPointModifyType::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old value.
	FUDDealPointValueData newData = UUDDealActionPointModifyType::ConvertData(action);
	FUDValueData oldData = UUDDealActionPointModifyType::ConvertBackupData(action);
	EUDPointType pointType = UUDDealActionPointModifyType::IntegerToPointType(oldData.Value);
	world->Deals[newData.DealId]->Points[newData.Point]->Type = pointType;
}
bool UUDDealActionPointModifyType::IsBackupRequired()
{
	return true;
}
void UUDDealActionPointModifyType::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDealPointValueData data = UUDDealActionPointModifyType::ConvertData(action);
	action.BackupValueParameters.Empty(0);
	int32 pointType = UUDDealActionPointModifyType::PointTypeToInteger(world->Deals[action.DealId]->Points[action.Point]->Type);
	action.BackupValueParameters.Add(pointType);
}