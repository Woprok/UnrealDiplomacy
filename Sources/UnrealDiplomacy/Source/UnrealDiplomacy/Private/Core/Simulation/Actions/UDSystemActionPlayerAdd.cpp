// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionPlayerAdd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDSystemActionPlayerAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player is added to the state.
	world->TurnData.FactionTurnOrder.Add(action.InvokerFactionId);
	world->Factions.Add(action.InvokerFactionId, UUDFactionState::CreateState(action.InvokerFactionId));
}

void UUDSystemActionPlayerAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player is removed from the state.
	world->TurnData.FactionTurnOrder.Remove(action.InvokerFactionId);
	world->Factions.Remove(action.InvokerFactionId);
}