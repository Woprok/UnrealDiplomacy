// Copyright Miroslav Valach
// TODO decide what to do with disconnected player during the lobby.
// For mid game disconnected he is replaced by AI.

#include "Core/Simulation/Actions/UDSystemActionPlayerRemove.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UDEPRECATED_UDSystemActionPlayerRemove::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player that can play is removed turn order.
	if (UUDGlobalData::GaiaFactionId != action.InvokerFactionId &&
		UUDGlobalData::ObserverFactionId != action.InvokerFactionId)
	{
		world->TurnData.FactionTurnOrder.Remove(action.InvokerFactionId);
	}
	// Faction is removed from the state.	
	world->Factions.Remove(action.InvokerFactionId);
}

void UDEPRECATED_UDSystemActionPlayerRemove::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player that can play is added to turn order.
	if (UUDGlobalData::GaiaFactionId != action.InvokerFactionId &&
		UUDGlobalData::ObserverFactionId != action.InvokerFactionId)
	{
		world->TurnData.FactionTurnOrder.Add(action.InvokerFactionId);
	}
	// Faction is added to the state.
	world->Factions.Add(action.InvokerFactionId, UUDFactionState::CreateState(action.InvokerFactionId));
	// TODO Requires backup recovery to unserialize original state.
}

void UDEPRECATED_UDSystemActionPlayerIgnore::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player that can play is removed turn order.
	if (UUDGlobalData::GaiaFactionId != action.InvokerFactionId &&
		UUDGlobalData::ObserverFactionId != action.InvokerFactionId)
	{
		world->TurnData.FactionTurnOrder.Remove(action.InvokerFactionId);
	}
	// Faction is marked as ignored / killed.
	//world->Players[action.InvokerFactionId]->IsIgnored = true;
}

void UDEPRECATED_UDSystemActionPlayerIgnore::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player that can play is added to turn order.
	if (UUDGlobalData::GaiaFactionId != action.InvokerFactionId &&
		UUDGlobalData::ObserverFactionId != action.InvokerFactionId)
	{
		world->TurnData.FactionTurnOrder.Add(action.InvokerFactionId);
	}
	// Faction is marked as ignored / killed.
	//world->Players[action.InvokerFactionId]->IsIgnored = false;
}