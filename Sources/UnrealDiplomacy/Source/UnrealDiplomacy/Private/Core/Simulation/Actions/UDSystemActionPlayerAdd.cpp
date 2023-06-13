// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionPlayerAdd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDSystemActionPlayerAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Faction that can play is added to turn order.
	if (UUDGlobalData::GaiaFactionId != action.InvokerFactionId &&
		UUDGlobalData::ObserverFactionId != action.InvokerFactionId)
	{
		world->TurnData.FactionTurnOrder.Add(action.InvokerFactionId);
	}
	// Faction is added to the state.
	world->Factions.Add(action.InvokerFactionId, UUDFactionState::CreateState(action.InvokerFactionId));
}

void UUDSystemActionPlayerAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Faction that can play is removed from turn order.
	if (UUDGlobalData::GaiaFactionId != action.InvokerFactionId &&
		UUDGlobalData::ObserverFactionId != action.InvokerFactionId)
	{
		world->TurnData.FactionTurnOrder.Remove(action.InvokerFactionId);
	}
	// Faction is removed from the state.
	world->Factions.Remove(action.InvokerFactionId);
}