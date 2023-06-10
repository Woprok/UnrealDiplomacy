// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSettingActionStratagemPointsChange.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDSettingActionStratagemOptionDeselect.h"

void UUDSettingActionStratagemPointsChange::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value
	FUDSettingDataValue data(action.ValueParameters);
	world->Settings.StratagemPoints = data.Value;
}

void UUDSettingActionStratagemPointsChange::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to backup value
	FUDSettingDataValue oldData(action.BackupValueParameters);
	world->Settings.StratagemPoints = oldData.Value;
}

void UUDSettingActionStratagemPointsChange::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDSettingDataValue data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(world->Settings.StratagemPoints);
}

TArray<FUDActionData> UUDSettingActionStratagemPointsChange::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	return GetDeselectForAll(world);
}

TArray<FUDActionData> UUDSettingActionStratagemPointsChange::GetDeselectForAll(TObjectPtr<UUDWorldState> world) const
{
	TArray<FUDActionData> finalActionList = { };
	for (auto faction : world->Factions)
	{
		finalActionList.Append(GetFactionDeselectAll(faction.Value));
	}
	return finalActionList;
}

TArray<FUDActionData> UUDSettingActionStratagemPointsChange::GetFactionDeselectAll(TObjectPtr<UUDFactionState> faction) const
{
	TArray<FUDActionData> factionList;
	for (auto stratagem : faction->StratagemOptions)
	{
		factionList.Add(FUDActionData(
			UUDSettingActionStratagemOptionDeselect::ActionTypeId,
			faction->PlayerUniqueId,
			{ stratagem } 
		));
	}
	return factionList;
}