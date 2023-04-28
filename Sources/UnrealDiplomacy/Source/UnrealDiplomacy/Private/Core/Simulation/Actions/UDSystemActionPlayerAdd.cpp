// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionPlayerAdd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

void UUDSystemActionPlayerAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player is added to the state.
	world->PlayerOrder.Add(action.InvokerPlayerId);
	world->Players.Add(action.InvokerPlayerId, UUDNationState::CreateState(action.InvokerPlayerId));
}

void UUDSystemActionPlayerAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player is removed from the state.
	world->PlayerOrder.Remove(action.InvokerPlayerId);
	world->Players.Remove(action.InvokerPlayerId);
}