// Copyright Miroslav Valach
// TODO this should probably not assume that everyone finished turn. Change it to backup...

#include "Core/Simulation/Actions/UDSystemActionIntermezzoEnd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDSystemActionIntermezzoEnd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isIntermezzo = world->TurnData.IsIntermezzo;
	bool isGaia = action.InvokerFactionId == UUDGlobalData::GaiaFactionId;
	return IUDActionInterface::CanExecute(action, world) && isIntermezzo && isGaia;
}

void UUDSystemActionIntermezzoEnd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Deactivate intermezzo.
	world->TurnData.IsIntermezzo = false;
	world->TurnData.RegentCreatedDeal = false;
	// Clear finish list
	world->TurnData.TurnFinishedFactions.Empty();
}

void UUDSystemActionIntermezzoEnd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revert intermezzo to active.
	world->TurnData.IsIntermezzo = true;
	// Fill list with all playing players:
	for (int32 faction : world->TurnData.FactionTurnOrder)
	{
		world->TurnData.TurnFinishedFactions.Add(faction);
	}
	// TODO proper revert for
	// world->TurnData.RegentCreatedDeal = false;
}