// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionPlayerRemove.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UDEPRECATED_UDSystemActionPlayerRemove::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player is removed from the state.
	world->TurnData.FactionTurnOrder.Remove(action.InvokerFactionId);
	world->Factions.Remove(action.InvokerFactionId);
}

void UDEPRECATED_UDSystemActionPlayerRemove::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player is added to the state.
	// TODO Requires backup recovery to unserialize original state.
	world->TurnData.FactionTurnOrder.Add(action.InvokerFactionId);
	world->Factions.Add(action.InvokerFactionId, UUDFactionState::CreateState(action.InvokerFactionId));
}

void UDEPRECATED_UDSystemActionPlayerIgnore::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player is removed from the state.
	world->TurnData.FactionTurnOrder.Remove(action.InvokerFactionId);
	//world->Players[action.InvokerFactionId]->IsIgnored = true;
}

void UDEPRECATED_UDSystemActionPlayerIgnore::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player is added to the state.
	// TODO Requires backup recovery to restore original position.
	world->TurnData.FactionTurnOrder.Add(action.InvokerFactionId);
	//world->Players[action.InvokerFactionId]->IsIgnored = false;
}