// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionPlayerRemove.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDSystemActionPlayerRemove::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player is removed from the state.
	world->PlayerOrder.Remove(action.InvokerPlayerId);
	world->Players.Remove(action.InvokerPlayerId);
}

void UUDSystemActionPlayerRemove::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player is added to the state.
	// TODO Requires backup recovery to unserialize original state.
	world->PlayerOrder.Add(action.InvokerPlayerId);
	world->Players.Add(action.InvokerPlayerId, UUDNationState::CreateState(action.InvokerPlayerId));
}

void UUDSystemActionPlayerIgnore::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player is removed from the state.
	world->PlayerOrder.Remove(action.InvokerPlayerId);
	//world->Players[action.InvokerPlayerId]->IsIgnored = true;
}

void UUDSystemActionPlayerIgnore::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player is added to the state.
	// TODO Requires backup recovery to restore original position.
	world->PlayerOrder.Add(action.InvokerPlayerId);
	//world->Players[action.InvokerPlayerId]->IsIgnored = false;
}