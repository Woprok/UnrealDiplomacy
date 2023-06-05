// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSettingActionMapHeightChange.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDSettingActionMapHeightChange::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value
	FUDSettingDataValue data(action.ValueParameters);
	world->Settings.MapHeight = data.Value;
}

void UUDSettingActionMapHeightChange::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to backup value
	FUDSettingDataValue oldData(action.BackupValueParameters);
	world->Settings.MapHeight = oldData.Value;
}

void UUDSettingActionMapHeightChange::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(world->Settings.MapHeight);
}