// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionFactionTakeover.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDSystemActionFactionTakeover::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change to new value.
	FUDSystemDataValue newData(action.ValueParameters);
	EUDFactionController factionController = UUDSystemActionFactionTakeover::IntegerToFactionController(newData.Value);
	world->Factions[action.InvokerFactionId]->Controller = factionController;
}

void UUDSystemActionFactionTakeover::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change to old value.
	FUDSystemDataValue newData(action.ValueParameters);
	FUDSystemDataValue oldData(action.BackupValueParameters);
	EUDFactionController factionController = UUDSystemActionFactionTakeover::IntegerToFactionController(oldData.Value);
	world->Factions[action.InvokerFactionId]->Controller = factionController;
}

void UUDSystemActionFactionTakeover::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDSystemDataValue data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	int32 factionController = UUDSystemActionFactionTakeover::FactionControllerToInteger(world->Factions[action.InvokerFactionId]->Controller);
	action.BackupValueParameters.Add(factionController);
}

int32 UUDSystemActionFactionTakeover::FactionControllerToInteger(EUDFactionController factionController)
{
	return static_cast<int32>(static_cast<uint8>(factionController));
}

EUDFactionController UUDSystemActionFactionTakeover::IntegerToFactionController(int32 factionControllerValue)
{
	if (0 > factionControllerValue || factionControllerValue > UINT8_MAX)
		return EUDFactionController::Error;
	return static_cast<EUDFactionController>(factionControllerValue);
}