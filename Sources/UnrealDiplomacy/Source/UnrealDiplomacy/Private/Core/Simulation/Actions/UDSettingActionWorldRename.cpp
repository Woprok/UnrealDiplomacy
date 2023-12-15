// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSettingActionWorldRename.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDSettingActionWorldRename::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value
	world->Factions[action.InvokerFactionId]->Name = action.TextParameter;
}

void UUDSettingActionWorldRename::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to backup value
	world->Factions[action.InvokerFactionId]->Name = action.BackupTextParameter;
}

void UUDSettingActionWorldRename::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old faction name is backuped for future revert use.
	action.BackupTextParameter = world->Factions[action.InvokerFactionId]->Name;
}